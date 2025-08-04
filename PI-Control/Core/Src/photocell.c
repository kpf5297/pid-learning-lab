#include "photocell.h"

static inline uint32_t map_range(uint32_t x, uint32_t in_min, uint32_t in_max,
                                 uint32_t out_min, uint32_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void set_pwm(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t value) {
    if (htim == NULL)
        return;
    __HAL_TIM_SET_COMPARE(htim, channel, value);
}

void photoCell_init(photoCell_t *sensor, ADC_HandleTypeDef *hadc) {
    sensor->scaled = true;
    sensor->current_level = 0.0f;
    sensor->last_raw_value = 0;
    sensor->min_value = 0;
    sensor->max_value = 4095;
    sensor->hadc = hadc;
    sensor->htim = NULL;
    sensor->pwm_channel = UINT32_MAX;
}

void photoCell_autoCalibrate(photoCell_t *sensor, TIM_HandleTypeDef *htim, uint32_t channel) {
    sensor->htim = htim;
    sensor->pwm_channel = channel;

    const int samples = 16;
    uint32_t sum_low = 0;
    uint32_t sum_high = 0;

    // Start PWM and drive LED fully on
    HAL_TIM_PWM_Start(htim, channel);
    set_pwm(htim, channel, htim->Init.Period); // full duty cycle
    HAL_Delay(100);
    for (int i = 0; i < samples; ++i) {
        if (HAL_ADC_Start(sensor->hadc) != HAL_OK ||
            HAL_ADC_PollForConversion(sensor->hadc, PHOTOCELL_ADC_TIMEOUT_MS) != HAL_OK)
            return;
        sum_high += HAL_ADC_GetValue(sensor->hadc);
    }

    // Turn LED fully off
    set_pwm(htim, channel, 0);
    HAL_Delay(100);
    for (int i = 0; i < samples; ++i) {
        if (HAL_ADC_Start(sensor->hadc) != HAL_OK ||
            HAL_ADC_PollForConversion(sensor->hadc, PHOTOCELL_ADC_TIMEOUT_MS) != HAL_OK)
            return;
        sum_low += HAL_ADC_GetValue(sensor->hadc);
    }

    sensor->max_value = sum_high / samples;
    sensor->min_value = sum_low / samples;
}

float readSensor(photoCell_t *sensor) {
    if (HAL_ADC_Start(sensor->hadc) != HAL_OK ||
        HAL_ADC_PollForConversion(sensor->hadc, PHOTOCELL_ADC_TIMEOUT_MS) != HAL_OK)
        return sensor->current_level;

    uint16_t raw = HAL_ADC_GetValue(sensor->hadc);
    sensor->last_raw_value = raw;

    if (sensor->scaled) {
        if (raw <= sensor->min_value) {
            sensor->current_level = 0.0f;
        } else if (raw >= sensor->max_value) {
            sensor->current_level = 100.0f;
        } else {
            sensor->current_level = (float)map_range(raw, sensor->min_value,
                                                     sensor->max_value, 0, 100);
        }
    } else {
        sensor->current_level = (float)raw;
    }

    return sensor->current_level;
}

