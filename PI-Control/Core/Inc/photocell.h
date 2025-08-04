#pragma once
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#define PHOTOCELL_ADC_TIMEOUT_MS 10U

typedef struct {
    bool scaled;              // output scaled to 0-100%
    float current_level;      // last scaled level
    uint16_t last_raw_value;  // last raw ADC reading
    uint16_t min_value;       // calibrated minimum raw value
    uint16_t max_value;       // calibrated maximum raw value
    ADC_HandleTypeDef *hadc;  // ADC handle used for measurements
    TIM_HandleTypeDef *htim;  // PWM timer handle for LED control
    uint32_t pwm_channel;     // PWM channel for LED control
} photoCell_t;

void photoCell_init(photoCell_t *sensor, ADC_HandleTypeDef *hadc);
void photoCell_autoCalibrate(photoCell_t *sensor, TIM_HandleTypeDef *htim, uint32_t channel);
float readSensor(photoCell_t *sensor);

// simple helper to set a PWM compare value
void set_pwm(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t value);

