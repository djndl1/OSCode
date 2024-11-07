#include "data_buffer.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>


allocation_result_t std_allocate(size_t count)
{
    allocation_result_t result = {
    .error = 0,
    .buffer = {
        .data = NULL,
        .length = 0,
        }
    };
    void *data = malloc(count);
    if (data == NULL) {
        result.error = errno;
        return result;
    }

    result.error = 0;
    result.buffer = DATA_BUFFER(data, count);

    return result;
}

void std_deallocate(data_buffer_t buf)
{
    free(buf.data);
}

error_t data_buffer_resize(data_buffer_t self, size_t newsize)
{
    void* newbuf = realloc(self.data, newsize);
    if (newbuf == NULL) {
        return ERROR(errno);
    }
    self.data = newbuf;
    self.length = newsize;

    return E_OK;
}

static bool buffers_overlapping(const data_buffer_t a, const data_buffer_t b)
{
    void *s1 = a.data;
    void *e1 = a.data + a.length;

    void *s2 = b.data;
    void *e2 = b.data + b.length;

    return MIN(e1, e2) - MAX(s1, s2) > 0;
}

bool data_buffer_compare(const data_buffer_t self, data_buffer_t other, size_t count)
{
    if (self.data == NULL && other.data == NULL
        || self.data != NULL && other.data == NULL) {
        return false;
    }

    size_t self_cmp_len = MIN(self.length, count);
    size_t other_cmp_len = MIN(other.length, count);

    if (self_cmp_len != other_cmp_len) return false;

    return memcmp(self.data, other.data, self_cmp_len) == 0;
}

error_t data_buffer_copy_to(const data_buffer_t self, data_buffer_t target)
{
    if (self.data == NULL || self.length == 0
         || target.data == NULL || target.length == 0) {
        return ERROR(EINVAL);
    }

    if (buffers_overlapping(self, target)) {
        return ERROR(E_OVERLAP_MEM);
    }

    size_t l = MIN(self.length, target.length);

    memcpy(target.data, self.data, l);

    return E_OK;
}
