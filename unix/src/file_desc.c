#include "file_desc.h"

#define _GNU_SOURCE

#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>

#if !defined(_GNU_SOURCE) || !defined(O_TMPFILE)
#define O_TMPFILE 0
#endif


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

file_desc_result_t file_open_at(const file_desc_t dir, const char *pathname, int flags)
{
    return (file_desc_result_t){ .error = ENOSYS };
}

file_desc_result_t file_create_at(const file_desc_t dir, const char *pathanem, int flags)
{
    return (file_desc_result_t){ .error = ENOSYS };
}

file_read_result_t file_read_into(const file_desc_t self, data_buffer_t buffer)
{
    if (buffer.data == NULL || buffer.length == 0) {
        return (file_read_result_t) { .error = EINVAL };
    }

    ssize_t n = read(self.fd, buffer.data, buffer.length);
    if (n == -1) {
        return (file_read_result_t) { .error = errno };
    }

    return (file_read_result_t) { .error = 0, .read_count = n, .buffer = buffer };
}

file_read_result_t file_read(const file_desc_t self, size_t count)
{
    if (count == 0) {
        return (file_read_result_t) { 0 };
    }

    buffer_alloc_result_t alloc_result = std_allocate_buffer(count);
    if (alloc_result.error != 0) {
        return (file_read_result_t) { .error = alloc_result.error };
    }
    data_buffer_t buf = alloc_result.buffer;

    file_read_result_t result = { 0 };

    file_read_result_t buf_result = file_read_into(self, buf);
    if (buf_result.error != 0) {
        result.error = buf_result.error;
        result.read_count = 0;
        goto deallocate_buf;
    }

    if (buf_result.read_count < buf.length) {
        error_t e = data_buffer_resize(buf, buf_result.read_count);
        if (e.error != 0) {
            result.error = buf_result.error;
            result.read_count = 0;
            goto deallocate_buf;
        }
    }

    result.error = 0;
    result.read_count = buf_result.read_count;
    result.buffer = buf;
    goto ret;

deallocate_buf:
    data_buffer_deallocate(buf);
ret:
    return result;
}

file_write_result_t file_write(const file_desc_t self,
                               const data_buffer_t buf)
{
    return file_write_until(self, buf, buf.length);
}

file_write_result_t file_write_until(const file_desc_t self,
                                     const data_buffer_t buf,
                                     size_t count)
{
    file_write_result_t result = { .error = 0, .written_count = 0 };
    if (buf.data == NULL || buf.length == 0 || count == 0) {
        result.error = EINVAL;
        return result;
    }

    size_t nwrite = MIN(buf.length, count);

    ssize_t n = write(self.fd, buf.data, nwrite);
    if (n == -1) {
        result.error = errno;
        return result;
    }

    result.written_count = n;
    result.error = 0;
    return result;
}

file_seek_result_t file_seek(const file_desc_t self, const file_seek_target_t target)
{
    file_seek_result_t result = { .error = 0, .offset = -1 };

    off_t of = lseek(self.fd, target.offset, target.whence);
    if (of == -1) {
        result.error = errno;
        return result;
    }

    result.offset = of;
    result.error = 0;
    return result;
}

error_t file_close(const file_desc_t self)
{
    int e = close(self.fd);
    if (e == -1) {
        return ERROR(e);
    }

    return E_OK;
}
