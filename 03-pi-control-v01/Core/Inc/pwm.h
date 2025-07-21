#ifndef PWM_H
#define PWM_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "logging.h"

/* Module version definitions. These are maintained alongside the
 * implementation so applications do not need to include a separate
 * version header. Bump the numbers whenever the API or behavior
 * changes in a backward-incompatible way. */
#define PWM_VERSION_MAJOR 1
#define PWM_VERSION_MINOR 0
#define PWM_VERSION_PATCH 0
#define PWM_VERSION_STRING "1.0.0"

/**
 * @brief Container describing a single PWM output channel.
 *
 * The structure keeps track of the STM32 timer, channel and cached
 * configuration values. Users typically allocate one structure per
 * output and pass it to the API functions.
 */
typedef struct {
    TIM_HandleTypeDef* htim;  /**< Initialized timer handle controlling the PWM */
    uint32_t channel;         /**< Timer channel (e.g. @ref TIM_CHANNEL_1) */
    uint32_t period;          /**< Timer period (ARR value) used for duty calc */
    uint8_t duty_percent;     /**< Last duty cycle that was programmed */
} PwmChannel_t;

/** Initialize a PWM channel instance.
 *  @param pwm       Pointer to the channel instance to initialize.
 *  @param htim      Timer handle previously configured for PWM output.
 *  @param channel   Specific timer channel constant (TIM_CHANNEL_x).
 */
void Pwm_init(PwmChannel_t* pwm, TIM_HandleTypeDef* htim, uint32_t channel);

/** Start PWM signal generation on the configured channel. */
void Pwm_start(PwmChannel_t* pwm);

/** Stop PWM signal generation. */
void Pwm_stop(PwmChannel_t* pwm);

/** Update the duty cycle percentage (0–100%). */
void Pwm_setDuty(PwmChannel_t* pwm, uint8_t duty_percent);

/** Obtain the compile-time module version string. */
const char* Pwm_getVersion(void);

#endif // PWM_H
