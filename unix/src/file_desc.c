#define _GNU_SOURCE

#include <errno.h>
#include <stdbool.h>
#include <fcntl.h>

#if !defined(_GNU_SOURCE) || !defined(O_TMPFILE)
#define O_TMPFILE 0
#endif

#include "file_desc.h"

file_desc_result file_open(const char *pathname, int flags)
{
    file_desc_result result = { .fd = { -1 }, .error = 0 };

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

file_desc_result file_create_impl(const char *pathname, int flags, mode_t modes, bool temp)
{
    file_desc_result result = { .fd = { -1 }, .error = 0 };
    int create_flags = flags | (temp ? O_TMPFILE : O_CREAT) | O_EXCL;

    int fd = open(pathname, create_flags, modes);
    if (fd == -1) {
        result.error = errno;
        return result;
    }

    result.fd = FILE_DESC(fd);
    result.error = 0;
    return result;
}

file_desc_result file_create(const char *pathname, int flags, mode_t modes)
{
    return file_create_impl(pathname, flags, modes, false);
}

file_desc_result file_open_at(const file_desc dir, const char *pathname, int flags)
{
    return (file_desc_result){ .error = ENOSYS };
}

file_desc_result file_create_at(const file_desc dir, const char *pathanem, int flags)
{
    return (file_desc_result){ .error = ENOSYS };
}

file_read_result file_read_into(const file_desc self, const data_buffer buffer)
{
    if (buffer.data == NULL || buffer.length == 0) {
        return (file_read_result) { .error = EINVAL };
    }

    ssize_t n = read(self.fd, buffer.data, buffer.length);
    if (n == -1) {
        return (file_read_result) { .error = errno };
    }

    return (file_read_result) { .error = 0, .read_count = n, .buffer = buffer };
}

file_read_result file_read(const file_desc self, size_t count)
{
    if (count == 0) {
        return (file_read_result) { 0 };
    }

    buffer_alloc_result alloc_result = std_allocate_buffer(count);
    if (alloc_result.error != 0) {
        return (file_read_result) { .error = alloc_result.error };
    }
    data_buffer buf = alloc_result.buffer;

    file_read_result result = { 0 };

    file_read_result buf_result = file_read_into(self, buf);
    if (buf_result.error != 0) {
        result.error = buf_result.error;
        result.read_count = 0;
        goto deallocate_buf;
    }

    if (buf_result.read_count < buf.length) {
        error_t e = data_buffer_resize(&buf, buf_result.read_count);
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
    data_buffer_destroy(&buf);
ret:
    return result;
}

file_write_result file_write(const file_desc self,
                             const data_buffer buf)
{
    return file_write_until(self, buf, buf.length);
}

file_write_result file_write_until(const file_desc self,
                                   const data_buffer buf,
                                   size_t count)
{
    file_write_result result = { .error = 0, .written_count = 0 };
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

file_seek_result file_seek(const file_desc self, const file_seek_target target)
{
    file_seek_result result = { .error = 0, .offset = -1 };

    off_t of = lseek(self.fd, target.offset, target.whence);
    if (of == -1) {
        result.error = errno;
        return result;
    }

    result.offset = of;
    result.error = 0;
    return result;
}

error_t file_close(file_desc *self)
{
    if (self == nullptr) {
        return ERR_FROM_CODE(EINVAL);
    }

    int e = close(self->fd);
    if (e == -1) {
        return ERR_FROM_CODE(e);
    }

    self->fd = -1;

    return E_OK;
}
