#include "test_utils.h"

#include "data_buffer.h"

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
    const char *msg_internal = strerror(error);
    char *msg = strdup(msg_internal);
    int n = 0;
    if (msg == NULL) {
        return -1;
    }

    deferred(free(msg)) {

        buffer_alloc_result_t buf_result = std_allocate_buffer(
            sizeof(char) * (strlen(msg) + strlen(fmt) + 3));
        if (buf_result.error != 0) {
            n = -1;
            break;
        }

        data_buffer_t buf = buf_result.buffer;
        deferred(data_buffer_deallocate(buf)) {

            snprintf(buf.data, buf.length, "%s: %s", msg, fmt);

            va_list vl;
            va_start(vl, fmt);
            n = vfprintf(stderr, buf.data, vl);
            va_end(vl);
        }
    }
    return n;
}
