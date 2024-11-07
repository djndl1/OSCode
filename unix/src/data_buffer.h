#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>
#include <stddef.h>

#include <stdbool.h>
#include "util.h"
#include "errors.h"

typedef struct {
    void *data;
    size_t length;
} data_buffer_t;

#define DATA_BUFFER(dat, len) \
    ((data_buffer_t){ .data = dat, .length = len })

#define data_buffer_element_at(self, typ, idx) \
    (((typ*)self.data)[idx])
#define byte_buffer_at(self, idx) data_buffer_element_at(self, uint8_t, idx)

typedef struct {
    int error;
    data_buffer_t buffer;
} allocation_result_t;

bool data_buffer_compare(const data_buffer_t self, data_buffer_t other, size_t count);

error_t data_buffer_resize(data_buffer_t self, size_t newsize);

error_t data_buffer_copy_to(const data_buffer_t, data_buffer_t);

allocation_result_t std_allocate(size_t);

void std_deallocate(data_buffer_t);



#endif // BUFFER_H_
