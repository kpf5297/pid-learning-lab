#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"  // HAL definitions

#define PHOTOCELL_VERSION "1.1.0"
// Default configuration macros (raw ADC range for 12-bit)
#define DEFAULT_SCALING    true
#define DEFAULT_MIN_READ    0
#define DEFAULT_MAX_READ   4095

// Maximum value passed to the PWM callback to request full brightness.
// Set this to match the range expected by your PWM driver.
#ifndef PHOTOCELL_PWM_TOP
#define PHOTOCELL_PWM_TOP 100U
#endif

typedef struct {
    bool scaled;              // Whether to scale result to 0–100
    uint16_t min_value;       // Raw ADC value mapped to "0%"
    uint16_t max_value;       // Raw ADC value mapped to "100%"
    uint8_t current_level;    // Scaled result (0–100 or 0–255)
    uint16_t last_raw_value;  // Most recent raw ADC value
} photoCell_t;

/**
 * Initializes the photocell with default or custom values.
 * @param sensor Pointer to photoCell_t struct to initialize.
 * @param scaled Whether to enable scaled output (0–100).
 * @param min_value Minimum raw value from the ADC.
 * @param max_value Maximum raw value from the ADC.
 */
void photoCell_init(photoCell_t* sensor, bool scaled, uint16_t min_value, uint16_t max_value);

/**
 * @brief Automatically calibrates the photoCell_t sensor using full PWM range.
 *
 * This function turns the LED to 100% and 0% brightness to measure the max/min
 * photocell response. It updates the sensor's min/max values accordingly.
 *
 * @param sensor Pointer to the photoCell_t struct to calibrate.
 * @param hadc Pointer to the ADC handle used for reading the sensor.
 * @param set_pwm Function to set PWM brightness (0–255). You must provide this.
 */
void photoCell_autoCalibrate(photoCell_t* sensor, ADC_HandleTypeDef* hadc, void (*set_pwm)(uint8_t));


/**
 * Reads the photocell sensor and updates current_level and last_raw_value.
 * @param sensor Pointer to photoCell_t struct.
 * @param hadc   Pointer to the ADC handle used for reading.
 * @return Scaled light level (0–100 if scaled, or raw clipped to 0–255).
 */
uint8_t readSensor(photoCell_t* sensor, ADC_HandleTypeDef* hadc);
