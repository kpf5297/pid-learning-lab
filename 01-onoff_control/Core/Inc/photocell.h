#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "logger.h"

// Default configuration macros (raw ADC range for 12-bit)
#define DEFAULT_SCALING    true
#define DEFAULT_MIN_READ    0
#define DEFAULT_MAX_READ   4095

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
 * Reads the photocell sensor and updates current_level and last_raw_value.
 * @param sensor Pointer to photoCell_t struct.
 * @return Scaled light level (0–100 if scaled, or raw clipped to 0–255).
 */
uint8_t readSensor(photoCell_t* sensor);
