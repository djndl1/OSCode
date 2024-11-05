#include "file_desc.h"

#include <errno.h>
#include <stdbool.h>

#define FILE_DESC(fd) ((file_desc_t){ .fd = fd })

file_desc_result_t file_open(const char *pathname, int flags)
{
    file_desc_result_t result = { .fd = { -1 }, .error = 0 };

    bool creating = flags & O_CREAT || flags & O_TMPFILE;
    if (creating) {
        result.error = EINVAL;
        return result;
    }

    int fd = open(pathname, flags);
    if (fd == -1) {
        result.error = errno;
        return result;
    }

    result.fd = FILE_DESC(fd);
    result.error = 0;
    return result;
}

file_desc_result_t file_create_impl(const char *pathname, int flags, mode_t modes, bool temp)
{
    file_desc_result_t result = { .fd = { -1 }, .error = 0 };
    int create_flags = flags | (temp ? O_TMPFILE : O_CREAT);

    int fd = open(pathname, create_flags, modes);
    if (fd == -1) {
        result.error = errno;
        return result;
    }

    result.fd = FILE_DESC(fd);
    result.error = 0;
    return result;
}

file_desc_result_t file_create(const char *pathname, int flags, mode_t modes)
{
    return file_create_impl(pathname, flags, modes, false);
}

file_desc_result_t file_open_at(const file_desc_t dir, const char *pathanem, int flags)
{
    return (file_desc_result_t){ .error = ENOSYS };
}

file_desc_result_t file_create_at(const file_desc_t dir, const char *pathanem, int flags)
{
    return (file_desc_result_t){ .error = ENOSYS };
}
