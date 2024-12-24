#ifndef WINALLOCATORS_H_
#define WINALLOCATORS_H_

#include <winhandle.h>

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const mem_allocator *const process_heap_allocator;

winhandle winheap_allocator_handle(const mem_allocator *const allocator);


typedef struct winheap_creation_options {
    size_t initial_size;
    size_t maximum_size;
    bool use_seh_exception;
    bool lockless;
    bool code_executable_from_heap;
} winheap_creation_options;

typedef struct winheap_creation_result {
    winstatus status;
    mem_allocator allocator;
} winheap_creation_result;

winheap_creation_result winheap_allocator_create(winheap_creation_options options);

winstatus winheap_allocator_destroy(mem_allocator *self);

typedef struct winheap_size_result {
    winstatus status;
    size_t size;
} winheap_size_result;

winheap_size_result winheap_allocator_largest_free_block_size(const mem_allocator *const self);

winheap_size_result winheap_allocator_committed_size(const mem_allocator* const self);

winheap_size_result winheap_allocator_allocated_size(const mem_allocator* const self);

winheap_size_result winheap_allocator_reserved_size(const mem_allocator* const self);

winheap_size_result winheap_allocator_max_reserved_size(const mem_allocator* const self);

winheap_size_result winheap_allocator_query_block_size(const mem_allocator* const self, const void* mem);

extern const mem_allocator *const global_allocator;

extern const mem_allocator *const local_allocator;

#ifdef __cplusplus
}
#endif

#endif // WIN32_ALLOCATORS_H_
