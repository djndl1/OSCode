#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>
#include "util.h"
#include "errors.h"
#include "allocator.h"

typedef struct {
    void *data;
    size_t length;

    const allocator_t *allocator;
} data_buffer_t;

#define DATA_BUFFER_TRANSIENT(dat, len) \
    ((data_buffer_t){ .data = dat, .length = len, .allocator = NULL })

#define data_buffer_element_at(self, typ, idx) \
    (((typ*)self.data)[idx])
#define byte_buffer_at(self, idx) data_buffer_element_at(self, uint8_t, idx)

typedef struct {
    int error;
    data_buffer_t buffer;
} buffer_alloc_result_t;

bool data_buffer_compare(const data_buffer_t self, data_buffer_t other, size_t count);

error_t data_buffer_resize(data_buffer_t self, size_t newsize);

error_t data_buffer_copy_to(const data_buffer_t, data_buffer_t);

buffer_alloc_result_t data_buffer_new(size_t count, const allocator_t *allocator);

buffer_alloc_result_t std_allocate_buffer(size_t);

void data_buffer_deallocate(data_buffer_t);



#endif // BUFFER_H_
