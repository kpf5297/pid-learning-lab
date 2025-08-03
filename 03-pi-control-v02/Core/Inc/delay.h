#pragma once
#include "stm32f4xx_hal.h"

#ifdef USE_FREERTOS
#include "cmsis_os.h"
/**
 * @brief Delay macro that cooperates with FreeRTOS scheduler.
 *
 * When USE_FREERTOS is defined, this macro maps to osDelay from
 * CMSIS-RTOS v1, providing a non-blocking delay that allows other
 * tasks to run. Otherwise it falls back to the standard HAL_Delay.
 */
#define PID_DELAY(ms) osDelay(pdMS_TO_TICKS(ms))
#else
#define PID_DELAY(ms) HAL_Delay(ms)
#endif
