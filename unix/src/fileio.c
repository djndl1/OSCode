#include "utest.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include <time.h>

#include "test_utils.h"

UTEST(FILE_IO, OPENMAX)
{
    struct utsname un = { 0 };
    uname(&un);

    printf("As of %s %s, "
           "OPEN_MAX (the maximum number of opened file descriptors) is now %ld\n",
           un.sysname, un.release, sysconf(_SC_OPEN_MAX));
}

UTEST(FILE_IO, OFFSET_T)
{
    printf("off_t size: %ld\n", sizeof(off_t));
}

UTEST(FILE_IO, HOLE)
{
    int fd = -1;

    if ((fd = open("file.hole", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG)) < 0) {
        perror("open error");
        ASSERT_FALSE_MSG(true, "failed to open: bailing out");
    }

    if ((write(fd, "ABC", 3)) != 3) {
        ASSERT_FALSE_MSG(true, "failed to write initial data, bailing out");
    }

    if (lseek(fd, 200000, SEEK_CUR) == -1) {
        ASSERT_FALSE_MSG(true, "failed to seek to the next position, bailing out");
    }

    if ((write(fd, "abc", 3)) != 3) {
        ASSERT_FALSE_MSG(true, "failed to write the second batch of data, bailing out");
    }

    close(fd);
}

UTEST(FILEIO, ILP_OFF)
{
    printf("Actual: int %ld, long %ld, offset %ld\n",
           sizeof(int) * 8, sizeof(long) * 8, sizeof(off_t) * 8);

    int ilp32_off32 = sysconf(_SC_V7_ILP32_OFF32);
    if (ilp32_off32) {
        printf("sysconf: int 32, long 32, offset 32\n");
        return;
    }

    int lp64_off64 = sysconf(_SC_V7_LP64_OFF64);
    if (lp64_off64) {
        printf("sysconf: int 32, long 64, offset 64\n");
        return;
    }
}

struct clock_errno {
    int error;
    clock_t time;
};
typedef struct clock_errno clock_errno_t;

clock_errno_t write_efficiency_test(size_t bufsize)
{
    clock_errno_t result = { 0 };

    const char *input_file = getenv("FILEIO_TEST_INPUT");
    if (input_file == NULL) {
        result.error = EINVAL;
        goto ret_result;
    }
    fprintf(stderr, "reading %s\n", input_file);

    int input_fd = open(input_file, O_RDONLY);
    if (input_fd == -1) {
        result.error = errno;
        goto ret_result;
    }

    void *buf = malloc(bufsize);
    if (buf == NULL) {
        result.error = errno;
        goto close_input;
    }

    stopwatch_t sw = stopwatch_new();
    stopwatch_start(&sw);
    int n = 0;
    while ((n = read(input_fd, buf, bufsize)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n) {
            result.error = errno;
            goto ret_result;
        }
    }

    if (n < 0) {
        result.error = errno;
        goto ret_result;
    }
    stopwatch_stop(&sw);
    result.time = stopwatch_elapsed_clocks(&sw);

deallocate_buf:
    if (!buf) {
        free(buf);
    }
close_input:
    if (input_fd >= 0) {
        close(input_fd);
    }
ret_result:
    return result;
}

UTEST(FILEIO, BUF_EFFICIENCY)
{
    size_t bufsizes[] = {
        1, 2, 4, 8, 16, 32, 64, 128, 256,
        512, 1024, 2048, 4096, 8192, 16384,
        32768, 65536, 131072, 262144, 524288
    };
    for (size_t i = 0; i < sizeof(bufsizes) / sizeof(size_t); i++) {
        size_t bufsize = bufsizes[i];
        clock_errno_t result = write_efficiency_test(bufsize);
        if (result.error) {
            const char *err_msg = strerror(result.error);
            fprintf(stderr, "bufsize %zu error: %s\n", bufsize, err_msg);
            continue;
        }
        double elapsed_secs = ((double)result.time / CLOCKS_PER_SEC);
        fprintf(stderr,
                "Bufsize %zu time consumption: %lf\n",
                bufsize, elapsed_secs);
    }
}

UTEST_MAIN();
