#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

#include <time.h>

struct _stopwatch {
    clock_t start;
    clock_t end;
};

typedef struct _stopwatch stopwatch;

stopwatch stopwatch_new();

void stopwatch_start(stopwatch *self);

void stopwatch_stop(stopwatch *self);

clock_t stopwatch_elapsed_clocks(const stopwatch *self);

double stopwatch_elapsed_seconds(const stopwatch *self);

int print_error(int error, const char *fmt, ...);

#endif // TEST_UTILS_H_
