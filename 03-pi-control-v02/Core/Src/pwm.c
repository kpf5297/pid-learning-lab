#include "pwm.h"

/*
 * PWM control module implementation. These routines provide a thin
 * abstraction layer over the STM32 HAL to make simple PWM output
 * configuration less error prone. Logging is optional and compiled
 * in only if the user supplies a logger implementation.
 */

/*
 * Initialize the PWM channel structure and start the timer in PWM mode.
 * The timer handle must already be configured with the desired period and
 * prescaler values prior to calling this function.
 */
void Pwm_init(PwmChannel_t* pwm, TIM_HandleTypeDef* htim, uint32_t channel) {
    pwm->htim = htim;
    pwm->channel = channel;
    pwm->period = PWM_TIMER_TOP;
    pwm->duty_percent = 0;

    HAL_TIM_PWM_Start(pwm->htim, pwm->channel);
}

/* Start PWM signal generation on the configured channel. */
void Pwm_start(PwmChannel_t* pwm) {
    HAL_TIM_PWM_Start(pwm->htim, pwm->channel);
}

/* Stop PWM output on the channel. */
void Pwm_stop(PwmChannel_t* pwm) {
    HAL_TIM_PWM_Stop(pwm->htim, pwm->channel);
}

/*
 * Update the pulse width to achieve the requested duty cycle. The duty
 * value is clipped to the range 0–100%.
 */
void Pwm_setDuty(PwmChannel_t* pwm, uint8_t duty_percent) {
    if (duty_percent > PWM_MAX_DUTY) duty_percent = PWM_MAX_DUTY;
    pwm->duty_percent = duty_percent;

    uint32_t pulse = (pwm->period * duty_percent) / PWM_MAX_DUTY;
    __HAL_TIM_SET_COMPARE(pwm->htim, pwm->channel, pulse);
}

/* Report the version string for this module. */
const char* Pwm_getVersion(void) {
    return PWM_VERSION_STRING;
}
