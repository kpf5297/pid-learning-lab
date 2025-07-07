/**
 * @file    pid.h
 * @brief   Tiny proportional-only controller suitable for embedded targets.
 *
 * Copyright (c) 2025 Kevin Fox
 * SPDX-License-Identifier: MIT
 *
 * Usage:
 *     #include "pid.h"
 *     pid_t ctrl = PID_DEFAULTS;
 *     PID_INIT(&ctrl);                 // or pid_init(&ctrl, …) for run-time tweaking
 *     …
 *     float u = PID_COMPUTE(&ctrl, measured_value);
 *     set_pwm_duty(u);                 // clamp already handled inside PID_COMPUTE
 */

#ifndef PID_H
#define PID_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------
 * Compile-time configuration
 * Override these with -D<NAME>=<value> on the compiler command line
 * or just edit them here.
 * ------------------------------------------------------------------*/
#ifndef PID_KP
#define PID_KP            1.0f      /**< Proportional gain */
#endif

#ifndef PID_SETPOINT
#define PID_SETPOINT      100.0f    /**< Desired process value */
#endif

#ifndef PID_OUT_MIN
#define PID_OUT_MIN       0.0f      /**< Lower clamp for controller output */
#endif

#ifndef PID_OUT_MAX
#define PID_OUT_MAX       100.0f    /**< Upper clamp for controller output */
#endif

/* --------------------------------------------------------------------
 * Data structure
 * ------------------------------------------------------------------*/
typedef struct
{
    float Kp;          /**< Proportional gain                  */
    float setpoint;    /**< Target value                       */
    float out_min;     /**< Minimum allowed controller output  */
    float out_max;     /**< Maximum allowed controller output  */
} pid_t;

/* Macro that yields a fully-initialised instance using the
 * default #defines above.  Example:
 *     pid_t led_ctrl = PID_DEFAULTS;
 */
#define PID_DEFAULTS \
{                     \
    .Kp        = PID_KP,          \
    .setpoint  = PID_SETPOINT,    \
    .out_min   = PID_OUT_MIN,     \
    .out_max   = PID_OUT_MAX      \
}

/* --------------------------------------------------------------------
 * API
 * ------------------------------------------------------------------*/

/**
 * @brief  Initialise (or re-initialise) a controller at run time.
 * @param  pid       Pointer to controller instance
 * @param  kp        Proportional gain
 * @param  setpoint  Desired process value
 * @param  out_min   Lower saturation limit
 * @param  out_max   Upper saturation limit
 */
void pid_init(pid_t *pid,
              float  kp,
              float  setpoint,
              float  out_min,
              float  out_max);

/**
 * @brief  Return the control effort for the current measurement.
 *         Output is clamped to [out_min, out_max].
 * @param  pid       Pointer to controller instance
 * @param  measured  Current process value
 * @return float     Controller output
 */
float pid_compute(const pid_t *pid, float measured);

/* -------------- Convenience macro (computes in-place) -------------- */
#define PID_COMPUTE(pid_ptr, meas)  pid_compute((pid_ptr), (meas))

#ifdef __cplusplus
}
#endif
#endif /* PID_H */
