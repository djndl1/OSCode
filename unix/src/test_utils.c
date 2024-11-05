#include "test_utils.h"

#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

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

int print_error(int error, const char *fmt, ...)
{
    const char *msg = strerror(error);
    size_t len = sizeof(char) * (strlen(msg) + strlen(fmt) + 3);
    char *buf = malloc(len);
    if (buf == NULL) {
        return 0;
    }
    snprintf(buf, len, "%s: %s", msg, fmt);

    va_list vl;
    va_start(vl, fmt);
    int n = vfprintf(stderr, buf, vl);
    va_end(vl);

free_buf:
    if (buf != NULL) {
        free(buf);
    }
    return n;
}
