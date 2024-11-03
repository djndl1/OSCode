#include "test_utils.h"

#include <time.h>

stopwatch_t stopwatch_new()
{
    stopwatch_t sw = { -1, -1 };

    return sw;
}

void stopwatch_start(stopwatch_t *self)
{
    self->start = clock();
}

void stopwatch_stop(stopwatch_t *self)
{
    self->end = clock();
}

clock_t stopwatch_elapsed_clocks(const stopwatch_t *self)
{
    if (self->end == -1) {
        return 0;
    }
    return self->end - self->start;
}

double stopwatch_elapsed_seconds(const stopwatch_t *self)
{
    clock_t clocks = stopwatch_elapsed_clocks(self);
    return ((double)clocks) / CLOCKS_PER_SEC;
}
