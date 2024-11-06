#include "data_buffer.h"

#include <stdlib.h>
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

error_t data_buffer_copy_to(const data_buffer_t self, data_buffer_t target)
{
    if (self.data == NULL || self.length == 0
         || target.data == NULL || target.length == 0) {
        return ERROR(EINVAL);
    }

    return ERROR(ENOSYS);
}
