#ifndef LED_PWM_H
#define LED_PWM_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "logger.h"

/**
 * @brief Maximum value accepted by ::LedPwm_setDuty.
 *
 * The default assumes a percentage based API (0-100).  Define this macro
 * prior to including this header if your application uses a different range.
 */
#ifndef LED_PWM_MAX_DUTY
#define LED_PWM_MAX_DUTY 100U
#endif

/**
 * @brief Timer top value used to generate 100% duty cycle.
 *
 * Set this to match the timer's auto-reload register (ARR) value.  The driver
 * no longer reads the value from the HAL timer structure so the user must
 * provide it explicitly.
 */
#ifndef LED_PWM_TIMER_TOP
#define LED_PWM_TIMER_TOP 100U
#endif

/**
 * @brief PWM-controlled LED interface.
 */
typedef struct {
    TIM_HandleTypeDef* htim;  /**< Pointer to the timer handle */
    uint32_t channel;         /**< Timer channel used for PWM (e.g., TIM_CHANNEL_1) */
    uint8_t duty_percent;     /**< Current duty cycle value (0–LED_PWM_MAX_DUTY) */
} LedPwm_t;

/**
 * @brief Initialize the LED PWM interface.
 * 
 * @param led Pointer to a LedPwm_t structure
 * @param htim Pointer to the hardware timer used for PWM
 * @param channel Timer channel used (TIM_CHANNEL_1, etc.)
 */
void LedPwm_init(LedPwm_t* led, TIM_HandleTypeDef* htim, uint32_t channel);

/**
 * @brief Start PWM output.
 * 
 * @param led Pointer to a LedPwm_t structure
 */
void LedPwm_start(LedPwm_t* led);

/**
 * @brief Stop PWM output.
 * 
 * @param led Pointer to a LedPwm_t structure
 */
void LedPwm_stop(LedPwm_t* led);

/**
 * @brief Set the PWM duty cycle.
 * 
 * @param led Pointer to a LedPwm_t structure
 * @param duty_percent Duty cycle in percent (0–100)
 */
void LedPwm_setDuty(LedPwm_t* led, uint8_t duty_percent);

#endif // LED_PWM_H
