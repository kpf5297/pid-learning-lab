#include "led_pwm.h"
#include "stm32f4xx_hal.h"

void LedPwm_init(LedPwm_t* led, TIM_HandleTypeDef* htim, uint32_t channel) {
    led->htim = htim;
    led->channel = channel;
    led->duty_percent = 0;
}

void LedPwm_start(LedPwm_t* led) {
    HAL_TIM_PWM_Start(led->htim, led->channel);
}

void LedPwm_stop(LedPwm_t* led) {
    HAL_TIM_PWM_Stop(led->htim, led->channel);
}

void LedPwm_setDuty(LedPwm_t* led, uint8_t duty_percent) {
    if (duty_percent > 100) duty_percent = 100;
    led->duty_percent = duty_percent;

    uint32_t period = led->htim->Init.Period;
    uint32_t pulse = (period * duty_percent) / 100;

    __HAL_TIM_SET_COMPARE(led->htim, led->channel, pulse);
}
