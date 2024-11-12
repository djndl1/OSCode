#include "dyn_cstr.h"

#include <string.h>

static dyn_cstr_result_t _create_dyn_cstr_from_data(const void *data,
                                                    size_t count,
                                                    const allocator_t *allocator)
{
     buffer_alloc_result_t alloc_res = data_buffer_new(count + 1, allocator);
     if (alloc_res.error) {
         return (dyn_cstr_result_t) { .error = alloc_res.error };
     }
     data_buffer_t buf = alloc_res.buffer;

     memcpy(buf.data, data, count);

     dyn_cstr_t str = { ._data = buf };
     dyn_cstr_at(str, count) = '\0';

     return (dyn_cstr_result_t) { .error = 0, .str = str };
}

static dyn_cstr_result_t _create_dyn_cstr_from_nbts(const char *nbts,
                                                    const allocator_t *allocator)
{
     size_t l = strlen(nbts);
     return  _create_dyn_cstr_from_data(nbts, l, allocator);
}

dyn_cstr_result_t dyn_cstr_from_buffer(data_buffer_t buf, const allocator_t *allocator)
{
    return _create_dyn_cstr_from_data(buf.data, buf.length, allocator);
}

dyn_cstr_result_t dyn_cstr_from(dyn_cstr_t str, const allocator_t *allocator)
{
    return _create_dyn_cstr_from_nbts(str._data.data, allocator);
}

dyn_cstr_result_t dyn_cstr_literal(const char *lit, const allocator_t *allocator)
{
    return _create_dyn_cstr_from_nbts(lit, allocator);
}

size_t dyn_cstr_len(dyn_cstr_t self)
{
    if (self._data.data == NULL || self._data.length == 0) {
        return 0;
    }

    return strlen(self._data.data);
}

int dyn_cstr_compare(dyn_cstr_t self, dyn_cstr_t other)
{
    return strcmp(self._data.data, other._data.data);
}

void dyn_cstr_destroy(dyn_cstr_t self)
{
    if (self._data.data == NULL || self._data.allocator == NULL) {
        return;
    }

    self._data.allocator->deallocate(self._data.allocator, self._data.data);
}
