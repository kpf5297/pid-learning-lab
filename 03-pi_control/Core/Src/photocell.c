#include "photocell.h"
#include "stm32f4xx_hal.h"  // HAL definitions for STM32F4
#include "logger.h"

/**
 * @brief Maps a value from one range to another.
 *
 * This is a general-purpose linear mapping function used to scale
 * ADC readings to a user-defined range (e.g., 0–100).
 *
 * @param x        Input value to be mapped.
 * @param in_min   Minimum of the input range.
 * @param in_max   Maximum of the input range.
 * @param out_min  Minimum of the output range.
 * @param out_max  Maximum of the output range.
 * @return long    Mapped output value.
 */
#define PHOTOCELL_ADC_TIMEOUT_MS 10U

static inline long map_range(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * @brief Initializes a photoCell_t instance with the specified configuration.
 *
 * This function sets the scaling behavior and raw ADC min/max calibration points.
 * It also clears the current light level and last raw ADC value.
 *
 * @param sensor      Pointer to a photoCell_t instance to initialize.
 * @param scaled      If true, the output will be scaled to a range of 0–100.
 * @param min_value   Minimum expected raw ADC value for scaling.
 * @param max_value   Maximum expected raw ADC value for scaling.
 */
void photoCell_init(photoCell_t* sensor, bool scaled, uint16_t min_value, uint16_t max_value) {
    sensor->scaled = scaled;
    sensor->min_value = min_value;
    sensor->max_value = max_value;
    sensor->current_level = 0;
    sensor->last_raw_value = 0;
}

void photoCell_autoCalibrate(photoCell_t* sensor, ADC_HandleTypeDef* hadc, void (*set_pwm)(uint8_t)) {
    const int samples = 16;
    uint32_t sum_low = 0;
    uint32_t sum_high = 0;

    // Set LED fully ON
    set_pwm(255);
    HAL_Delay(100);  // allow sensor to settle
    for (int i = 0; i < samples; i++) {
        if (HAL_ADC_Start(hadc) != HAL_OK ||
            HAL_ADC_PollForConversion(hadc, PHOTOCELL_ADC_TIMEOUT_MS) != HAL_OK) {
            Log(LOG_LEVEL_ERROR, "ADC read failed during high calibration\n");
            return;
        }
        sum_high += HAL_ADC_GetValue(hadc);
    }

    // Set LED fully OFF
    set_pwm(0);
    HAL_Delay(100);  // allow sensor to settle
    for (int i = 0; i < samples; i++) {
        if (HAL_ADC_Start(hadc) != HAL_OK ||
            HAL_ADC_PollForConversion(hadc, PHOTOCELL_ADC_TIMEOUT_MS) != HAL_OK) {
            Log(LOG_LEVEL_ERROR, "ADC read failed during low calibration\n");
            return;
        }
        sum_low += HAL_ADC_GetValue(hadc);
    }

    uint16_t avg_low = sum_low / samples;
    uint16_t avg_high = sum_high / samples;

    // Apply to sensor
    if (avg_high > avg_low + 10) {  // sanity check
        sensor->min_value = avg_low;
        sensor->max_value = avg_high;
    }
}


/**
 * @brief Reads the photocell value from ADC and updates the sensor state.
 *
 * This function performs a blocking ADC read using STM32 HAL and stores both
 * the raw and optionally scaled light level into the provided sensor instance.
 *
 * @param sensor  Pointer to a photoCell_t instance to update.
 * @return uint8_t  Scaled (0–100) or raw (0–255 clipped) light level.
 */
uint8_t readSensor(photoCell_t* sensor, ADC_HandleTypeDef* hadc) {
    if (HAL_ADC_Start(hadc) != HAL_OK ||
        HAL_ADC_PollForConversion(hadc, PHOTOCELL_ADC_TIMEOUT_MS) != HAL_OK) {
        Log(LOG_LEVEL_ERROR, "ADC read failed\n");
        return sensor->current_level;
    }
    uint16_t raw = HAL_ADC_GetValue(hadc);

    sensor->last_raw_value = raw;

    uint8_t value;
    if (sensor->scaled) {
        if (raw <= sensor->min_value) {
            value = 0;
        } else if (raw >= sensor->max_value) {
            value = 100;
        } else {
            value = (uint8_t)map_range(raw, sensor->min_value, sensor->max_value, 0, 100);
        }
    } else {
        value = (raw > 255) ? 255 : (uint8_t)raw;
    }

    sensor->current_level = value;
    return value;
}
