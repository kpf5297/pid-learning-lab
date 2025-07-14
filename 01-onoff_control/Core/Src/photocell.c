#include "photocell.h"
#include "stm32f4xx_hal.h"  // or whatever matches your MCU

extern ADC_HandleTypeDef hadc1;  // Your global ADC handle

static inline long map_range(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void photoCell_init(photoCell_t* sensor, bool scaled, uint16_t min_value, uint16_t max_value) {
    sensor->scaled = scaled;
    sensor->min_value = min_value;
    sensor->max_value = max_value;
    sensor->current_level = 0;
    sensor->last_raw_value = 0;

    Log(LOG_LEVEL_DEBUG, "Photocell initialized: scaled=%s, min=%d, max=%d\n", 
        sensor->scaled ? "true" : "false", sensor->min_value, sensor->max_value);
}

uint8_t readSensor(photoCell_t* sensor) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    uint16_t raw = HAL_ADC_GetValue(&hadc1);

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

    Log(LOG_LEVEL_DEBUG, "Photocell read: raw=%d, scaled=%s, value=%d\n", 
        raw, sensor->scaled ? "true" : "false", value);
        
    return value;
}
