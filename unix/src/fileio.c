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
#include "file_desc.h"

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
    file_desc_result_t result = file_create("file.hole",
                                            O_RDWR | O_TRUNC,
                                            S_IRWXU | S_IRWXG);
    if (result.error != 0) {
        print_error(result.error, "open error");
        ASSERT_FALSE_MSG(true, "failed to open: bailing out");
    }
    file_desc_t file = result.fd;

    file_write_result_t write_result = file_write(file,
                                                  DATA_BUFFER("ABC", 3));
    if (write_result.error != 0 || write_result.written_count != 3) {
        ASSERT_FALSE_MSG(true, "failed to write initial data, bailing out");
    }

    if (file_seek(file, SEEK_FROM_END(20000)).error != 0) {
        ASSERT_FALSE_MSG(true, "failed to seek to the next position, bailing out");
    }

    write_result = file_write(file, DATA_BUFFER("abc", 3));
    if (write_result.error != 0 || write_result.written_count != 3) {
        ASSERT_FALSE_MSG(true, "failed to write the second batch of data, bailing out");
    }

    file_close(file);
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

UTEST(FILEIO, APPEND_ALWAYS)
{
    const char *append_file = getenv("FILEIO_TEST_APPEND");
    if (append_file == NULL) {
        ASSERT_FALSE_MSG(true, "failed to get test append file");
    }

    file_desc_result_t open_result = file_create(append_file, O_APPEND | O_RDWR, S_IRWXU | S_IRWXG);
    if (open_result.error != 0) {
        goto ret;
    }
    file_desc_t file = open_result.fd;

    if (file_write(file, DATA_BUFFER("ABC", 3)).written_count < 3) {
        fprintf(stderr, "failed to write ABC\n");
        goto close_file;
    }

    if (file_seek(file, SEEK_FROM_BEGINNING(0)).offset > 0) {
        fprintf(stderr, "failed to seek to the beginning\n");
        goto close_file;
    }

    if (file_write(file, DATA_BUFFER("abc", 3)).written_count < 3) {
        fprintf(stderr, "failed to write abc\n");
        goto close_file;
    }

    if (file_seek(file, SEEK_FROM_BEGINNING(0)).offset > 0) {
        fprintf(stderr, "failed to seek to the beginning\n");
        goto close_file;
    }

    data_buffer_t buf = { 0 };
    bool failed_read = false;
    scoped (buf = file_read(file, 6).buffer, std_deallocate(buf)) {
        if (buf.length < 6) {
            failed_read = true;
            break;
        }

        error_t e = data_buffer_resize(buf, 7);
        if (e.error != 0) {
            //print_error(e.error, "Bytes read, resize failed\n");
            fprintf(stderr, "Bytes read, resize failed: %s, %d\n", strerror(e.error), e.error);
            goto close_file;
        }
        byte_buffer_at(buf, 6) = '\0';
    }

    if (failed_read) {
        fprintf(stderr, "failed to read all bytes\n");
        goto close_file;
    }

    ASSERT_EQ_MSG(strcmp("ABCabc", (char*)buf.data), 0, (char*)buf.data);

close_file:
    file_close(file);
ret:
    return;
}

UTEST_MAIN();
