#include <gnu/libc-version.h>

#include "utest.h"

UTEST(GLIBC, RUNTIME_VERSION)
{
    const char *ver = gnu_get_libc_version();
    const char *release = gnu_get_libc_release();

    printf("platform glibc version: %s, %s\n", ver, release);
}

UTEST(GLIBC, TARGET_VERSION) {
    printf("target glibc version: %d.%d\n",
           __GLIBC__, __GLIBC_MINOR__);
}

UTEST_MAIN();
