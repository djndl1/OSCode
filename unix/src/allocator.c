#include "allocator.h"

#include <stdlib.h>
#include <errno.h>

mem_alloc_result_t std_allocator_func(const allocator_t *const self, size_t count)
{
    void *mem = malloc(count);
    if (mem == NULL) {
        return (mem_alloc_result_t) { .error = errno, .mem = NULL };
    }

    return (mem_alloc_result_t) { .error = 0, .mem = mem };
}

void std_deallocator_func(const allocator_t *const self, void *mem)
{
    free(mem);
}

mem_alloc_result_t std_reallocator_func(const allocator_t *const self, void *mem, size_t newsize)
{
    void *newmem = realloc(mem, newsize);
    if (newmem == NULL) {
        return (mem_alloc_result_t) { .error = errno, .mem = NULL };
    }

    return (mem_alloc_result_t) { .error = 0, .mem = newmem };
}


static const allocator_t _std_allocator_table = {
    .user_data = NULL,
    .allocate = std_allocator_func,
    .deallocate = std_deallocator_func,
    .reallocate = std_reallocator_func,
};

const allocator_t *const std_allocator = &_std_allocator_table;
