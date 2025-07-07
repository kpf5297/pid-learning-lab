/**
 * @file    pid.c
 * @brief   Implementation of proportional-only controller.
 */

#include "pid.h"

/* ----------------------------- Helpers ----------------------------- */
static inline float pid_clamp(float v, float lo, float hi)
{
    if (v > hi) return hi;
    if (v < lo) return lo;
    return v;
}

/* --------------------------- Public API ---------------------------- */
void pid_init(pid_t *pid,
              float  kp,
              float  setpoint,
              float  out_min,
              float  out_max)
{
    pid->Kp        = kp;
    pid->setpoint  = setpoint;
    pid->out_min   = out_min;
    pid->out_max   = out_max;
}

float pid_compute(const pid_t *pid, float measured)
{
    float error  = pid->setpoint - measured;
    float output = pid->Kp * error;
    return pid_clamp(output, pid->out_min, pid->out_max);
}
