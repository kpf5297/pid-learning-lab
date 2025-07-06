#ifndef LED_PWM_H
#define LED_PWM_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

/**
 * @brief PWM-controlled LED interface.
 */
typedef struct {
    TIM_HandleTypeDef* htim;  /**< Pointer to the timer handle */
    uint32_t channel;         /**< Timer channel used for PWM (e.g., TIM_CHANNEL_1) */
    uint8_t duty_percent;     /**< Current duty cycle percentage (0–100) */
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
