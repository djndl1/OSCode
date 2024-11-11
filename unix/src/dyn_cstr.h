#ifndef DYN_CSTR_H_
#define DYN_CSTR_H_

#include "data_buffer.h"
#include "allocator.h"

typedef struct {
    data_buffer_t _data;
} dyn_cstr_t;

typedef struct {
    int error;
    dyn_cstr_t str;
} dyn_cstr_result_t;

#define dyn_cstr_nbts(str) ((char*)(str._data.data))
#define dyn_cstr_at(str, idx) ((dyn_cstr_nbts(str))[idx])

dyn_cstr_result_t dyn_cstr_from_buffer(data_buffer_t buf, const allocator_t *allocator);

dyn_cstr_result_t dyn_cstr_from(dyn_cstr_t str, const allocator_t *allocator);

dyn_cstr_result_t dyn_cstr_literal(const char *lit, const allocator_t *allocator);


#endif // DYN_CSTR_H_
