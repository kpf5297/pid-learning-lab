/**
 * @file    pid.c
 * @brief   Implementation of proportional-integral controller.
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
              float  ki,
              float  setpoint,
              float  out_min,
              float  out_max)
{
    pid->Kp        = kp;
    pid->Ki        = ki;
    pid->setpoint  = setpoint;
    pid->integral  = 0.0f;
    pid->out_min   = out_min;
    pid->out_max   = out_max;
}

float pid_compute(pid_t *pid, float measured)
{
    float error = pid->setpoint - measured;
    pid->integral += error;
    float output = pid->Kp * error + pid->Ki * pid->integral;
    return pid_clamp(output, pid->out_min, pid->out_max);
}
