#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utest.h"



UTEST(ERROR, SYSCALL)
{
    int res = open("./afd", O_RDONLY);

    if (res == -1) {
        int open_error = errno;
        perror("ERROR SYSCALL open: ");

        char *buf = (char*)malloc(1024);
        if (buf == NULL) {
            EXPECT_TRUE_MSG(false, "Failed to allocate a buffer");
        }

        int e = strerror_r(open_error, buf, 1024); // this doesn't set errno, good design
        if (e == 0) {
            printf("open error: %s\n", buf);
        }

buf_cleanup:
        free(buf);
        buf = NULL;
    } else {
        EXPECT_TRUE_MSG(false, "unlikely here");
    }
}

UTEST_MAIN();
