#ifndef TIMESPAN_H_
#define TIMESPAN_H_

#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct {
    int64_t _ticks;
} timespan_t;

#define TS_TICK_RESOLUTION 100
#define TS_TICKS_PER_MICROSECOND 10
#define TS_TICKS_PER_MILLISECOND (10 * TS_TICKS_PER_MICROSECOND)
#define TS_TICKS_PER_SECOND (1000UL * TS_TICKS_PER_MILLISECOND)
#define TS_TICKS_PER_MINUTE (60UL * TS_TICKS_PER_SECOND)
#define TS_TICKS_PER_HOUR (60UL * TS_TICKS_PER_MINUTE)
#define TS_TICKS_PER_DAY (24UL * TS_TICKS_PER_HOUR)

#define TIMESPAN_ZERO ((timspan_t){ ._ticks = 0 })
#define TIMESPAN_MIN ((timspan_t){ ._ticks = INT64_MAX })
#define TIMESPAN_MAX ((timspan_t){ ._ticks = INT64_MIN })
#define TIMESPAN_SECOND(n) timespan_from_ticks(n * TS_TICKS_PER_SECOND)
#define TIMESPAN_MINUTE(n) timespan_from_ticks(n * TS_TICKS_PER_MINUTE)
#define TIMESPAN_HOUR(n) timespan_from_ticks(n * TS_TICKS_PER_HOUR)
#define TIMESPAN_DAY(n) timespan_from_ticks(n * TS_TICKS_PER_DAY)
#define TIMESPAN(d, h, m, secs, msecs, usecs) _timespan_from(d, h, m, secs, msecs, usecs)

static inline timespan_t _timespan_from(int d, int h, int m, int64_t secs, int64_t msecs, int64_t usecs)
{
    int64_t dticks = TS_TICKS_PER_DAY * (int64_t)d;
    int64_t hticks = TS_TICKS_PER_HOUR * (int64_t)h;
    int64_t mticks = TS_TICKS_PER_MINUTE * (int64_t)m;
    int64_t secsticks = TS_TICKS_PER_SECOND * (int64_t)secs;
    int64_t msecsticks = TS_TICKS_PER_MILLISECOND * (int64_t)msecs;
    int64_t usecsticks = TS_TICKS_PER_MICROSECOND * (int64_t)usecs;

    int64_t ticks = dticks + hticks + mticks + secticks + msecticks + usecticks;

    return (timespan_t) { ._ticks = ticks };
}

static inline timespan_t timespan_from_ticks(int64_t ticks)
{
    return (timespan_t){ ._ticks = ticks };
}

static inline timespan_t timespan_add(timespan_t self, timespan_t b)
{
    return (timespan_t){ ._ticks = self._ticks + b._ticks };
}

static inline timespan_t timespan_minus(timespan_t self, timespan_t b)
{
    return (timespan_t){ ._ticks = self._ticks - b._ticks };
}

static inline timespan_t timespan_times(timespan_t self, double factor)
{
    return (timespan_t){ ._ticks = self._ticks * factor };
}

static inline timespan_t timespan_divided_by(timespan_t self, double factor)
{
    return (timespan_t){ ._ticks = (double)self._ticks / factor };
}

static inline timespan_t timespan_negative(timespan_t self)
{
    return (timespan_t){ ._ticks = -self._ticks };
}

static inline int timespan_compare(timespan_t self, timespan_t other)
{
    return self._ticks == other._ticks ? 0
        : (self._ticks < other._ticks ? -1 : 1);
}

static inline int64_t timespan_ticks(timespan_t ts)
{
    return ts._ticks;
}

static inline int timespan_day_part(timespan_t ts)
{
    return ts._ticks / TS_TICKS_PER_DAY;
}

static inline int timespan_hour_part(timespan_t ts)
{
    return (ts._ticks % TS_TICKS_PER_DAY) / TS_TICKS_PER_HOUR;
}

static inline int timespan_minute_part(timespan_t ts)
{
    return (ts._ticks % TS_TICKS_PER_HOUR) / TS_TICKS_PER_MINUTE;
}

static inline int timespan_second_part(timespan_t ts)
{
    return (ts._ticks % TS_TICKS_PER_MINUTE) / TS_TICKS_PER_SECOND;
}

static inline int timespan_millisecond_part(timespan_t ts)
{
    return (ts._ticks % TS_TICKS_PER_SECOND) / TS_TICKS_PER_MILLISECOND;
}

static inline int timespan_microsecond_part(timespan_t ts)
{
    return (ts._ticks % TS_TICKS_PER_MILLISECOND) / TS_TICKS_PER_MICROSECOND;
}

static inline timespan_t timespan_duration(timespan_t ts)
{
    return (timespan_t) { ._ticks = imaxabs(ts._ticks) };
}

static inline double timespan_as_microseconds(timespan_t ts)
{
    return (double)ts._ticks / TS_TICKS_PER_MICROSECOND;
}

static inline double timespan_as_milliseconds(timespan_t ts)
{
    return (double)ts._ticks / TS_TICKS_PER_MILLISECOND;
}

static inline double timespan_as_seconds(timespan_t ts)
{
    return (double)ts._ticks / TS_TICKS_PER_SECOND;
}

static inline double timespan_as_nanoseconds(timespan_t ts)
{
    return (double)ts._ticks * TS_TICK_RESOLUTION;
}

static inline struct timespec timespan_as_timespec(timespan_t ts)
{
    time_t secs = ts._ticks / TS_TICKS_PER_SECOND;
    unsigned long nsecs = (ts._ticks % TS_TICKS_PER_SECOND) * TS_TICK_RESOLUTION;
    return (struct timespec) { .tv_sec = secs, .tv_nsec = nsecs };
}

static inline clock_t timespan_as_clock(timespan_t ts)
{
    return CLOCKS_PER_SEC * timespan_as_seconds(ts);
}

static inline struct timeval timespan_as_timeval(timespan_t ts)
{
    time_t secs = ts._ticks / TS_TICKS_PER_SECOND;
    unsigned long usecs = (ts._ticks % TS_TICKS_PER_SECOND) / TS_TICKS_PER_MICROSECOND;
    return (struct timeval) { .tv_sec = secs, .tv_usec = usecs };
}

// TODO format, parsing

#endif // TIMESPAN_H_
