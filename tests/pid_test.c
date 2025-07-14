#include <assert.h>
#include <math.h>
#include "../02-proportional-control/Core/Inc/pid.h"

int main(void) {
    pid_t pid;
    pid_init(&pid, 1.0f, 100.0f, 0.0f, 100.0f);

    float out = pid_compute(&pid, 100.0f);
    assert(fabsf(out - 0.0f) < 1e-6);

    out = pid_compute(&pid, 0.0f);
    assert(fabsf(out - 100.0f) < 1e-6);

    out = pid_compute(&pid, 200.0f);
    assert(fabsf(out - 0.0f) < 1e-6);

    return 0;
}
