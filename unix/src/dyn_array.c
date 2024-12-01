#if !defined(dyn_array_element_type) || !defined(dyn_array_type_name)
#error "dynamic array type or dynamic array name not defined!"
#endif

#include "dyn_array_macro.h"
#include <string.h>



dyn_array_result_type_name dyn_array_func(from_data)(const void *data,
                                                     size_t count,
                                                     const allocator_t *allocator)
{
     size_t elem_size = sizeof(dyn_array_element_type);
     buffer_alloc_result_t alloc_res = data_buffer_new(count * elem_size, allocator);
     if (alloc_res.error) {
         return (dyn_array_result_type_name) { .error = alloc_res.error };
     }
     data_buffer_t buf = alloc_res.buffer;

     memcpy(buf.data, data, count * elem_size);

     dyn_array_type_name arr = { ._data = buf };
     arr._len = count;

     return (dyn_array_result_type_name) { .error = 0, .array = arr };
}

dyn_array_result_type_name dyn_array_func(from_buffer)(const data_buffer_t data,
                                                       const allocator_t *allocator)
{
    return dyn_array_func(from_data)(data.data, data.length, allocator);
}

void dyn_array_func(destroy)(dyn_array_type_name self)
{
    if (self._data.data == NULL || self._data.allocator == NULL) {
        return;
    }

    self._data.allocator->deallocate(self._data.allocator, self._data.data);
}

#undef dyn_array_element_type
#undef dyn_array_type_name
