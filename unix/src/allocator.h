#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <stddef.h>

typedef struct {
    int error;
    void *mem;
} mem_alloc_result_t;

struct _allocator;
typedef struct _allocator allocator_t;

struct _allocator {
    void *user_data;

    mem_alloc_result_t (*allocate)(const allocator_t * const self, size_t count);
    void (*deallocate)(const allocator_t *const self, void *mem);
    mem_alloc_result_t (*reallocate)(const allocator_t *const self, void *mem, size_t newsize);
};

extern const allocator_t *const std_allocator;

#endif // ALLOCATOR_H_
