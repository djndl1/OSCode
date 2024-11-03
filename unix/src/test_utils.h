#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <time.h>

struct _stopwatch {
    clock_t start;
    clock_t end;
};

typedef struct _stopwatch stopwatch_t;

stopwatch_t stopwatch_new();

void stopwatch_start(stopwatch_t *self);

void stopwatch_stop(stopwatch_t *self);

clock_t stopwatch_elapsed_clocks(const stopwatch_t *self);

double stopwatch_elapsed_seconds(const stopwatch_t *self);

#endif // TEST_UTILS_H_
