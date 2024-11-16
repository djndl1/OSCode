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
#include "dyn_cstr.h"

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
                                                  DATA_BUFFER_TRANSIENT("ABC", 3));
    if (write_result.error != 0 || write_result.written_count != 3) {
        ASSERT_FALSE_MSG(true, "failed to write initial data, bailing out");
    }

    if (file_seek(file, SEEK_FROM_END(20000)).error != 0) {
        ASSERT_FALSE_MSG(true, "failed to seek to the next position, bailing out");
    }

    write_result = file_write(file, DATA_BUFFER_TRANSIENT("abc", 3));
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

    file_desc_result_t input_fd = file_open(input_file, O_RDONLY);
    if (input_fd.error != 0) {
        result.error = input_fd.error;
        goto ret_result;
    }
    file_desc_t file = input_fd.fd;

    deferred(file_close(file)) {

        buffer_alloc_result_t alloc_res = std_allocate_buffer(bufsize);
        if (alloc_res.error != 0) {
            result.error = alloc_res.error;
            goto end_of_file_block;
        }
        stopwatch_t sw = stopwatch_new();

        bool io_op_error = false;
        data_buffer_t buf;
        scoped(buf = alloc_res.buffer, data_buffer_deallocate(buf)) {
            stopwatch_start(&sw);

            file_read_result_t read_res = { 0 };
            while ((read_res = file_read_into(file, buf)),
                   read_res.error == 0 && read_res.read_count > 0) {
                if (file_write_until(fd_stdout, buf, read_res.read_count).written_count != read_res.read_count) {
                    // write error
                    result.error = errno;
                    io_op_error = true;
                    goto end_of_buf_block;
                }
            }

            // read error
            if (read_res.error != 0) {
                result.error = read_res.error;
                io_op_error = true;
                break;
            }
        end_of_buf_block: ;
        }

        if (!io_op_error) {
            stopwatch_stop(&sw);
            result.time = stopwatch_elapsed_clocks(&sw);
        }

    end_of_file_block: ;
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
            print_error(result.error, "bufsize %zu \n", bufsize);
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
        print_error(open_result.error, "failed to open\n");
        goto ret;
    }
    file_desc_t file = open_result.fd;

    if (file_write(file, DATA_BUFFER_TRANSIENT("ABC", 3)).written_count < 3) {
        fprintf(stderr, "failed to write ABC\n");
        goto close_file;
    }

    if (file_seek(file, SEEK_FROM_BEGINNING(0)).offset > 0) {
        fprintf(stderr, "failed to seek to the beginning\n");
        goto close_file;
    }

    if (file_write(file, DATA_BUFFER_TRANSIENT("abc", 3)).written_count < 3) {
        fprintf(stderr, "failed to write abc\n");
        goto close_file;
    }

    if (file_seek(file, SEEK_FROM_BEGINNING(0)).offset > 0) {
        fprintf(stderr, "failed to seek to the beginning\n");
        goto close_file;
    }

    {
        data_buffer_t buf = { 0 };
        scoped (buf = file_read(file, 6).buffer, data_buffer_deallocate(buf)) {
            if (buf.length < 6) {
                fprintf(stderr, "failed to read all bytes\n");
                break;
            }
            {
                dyn_cstr_t s;
                scoped(s = dyn_cstr_from_buffer_stdalloc(buf).str, dyn_cstr_destroy(s)) {
                    const data_buffer_t actual_stored_data = DATA_BUFFER_TRANSIENT("ABCabc", 6);
                    printf("%s\n", dyn_cstr_nbts(s));
                    ASSERT_TRUE_MSG(data_buffer_compare(buf, actual_stored_data, buf.length),
                                    dyn_cstr_nbts(s));
                }
            }
        }
    }
close_file:
    file_close(file);
delete_file:
    unlink(append_file);
ret:
    return;
}

UTEST_MAIN();
