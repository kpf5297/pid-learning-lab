#include <assert.h>
#include <math.h>
#include "../03-pi-control/Core/Inc/pid.h"

int main(void) {
    pid_t pid;

    // Proportional-only behaviour
    pid_init(&pid, 1.0f, 0.0f, 100.0f, 0.0f, 100.0f);
    float out = pid_compute(&pid, 100.0f);
    assert(fabsf(out - 0.0f) < 1e-6);

    out = pid_compute(&pid, 0.0f);
    assert(fabsf(out - 100.0f) < 1e-6);

    out = pid_compute(&pid, 200.0f);
    assert(fabsf(out - 0.0f) < 1e-6);

    // Integral accumulation behaviour
    pid_init(&pid, 0.0f, 0.5f, 10.0f, 0.0f, 100.0f);
    out = pid_compute(&pid, 0.0f);   // error=10 -> integral=10 -> output=5
    assert(fabsf(out - 5.0f) < 1e-6);

    out = pid_compute(&pid, 0.0f);   // integral=20 -> output=10
    assert(fabsf(out - 10.0f) < 1e-6);

    return 0;
}
