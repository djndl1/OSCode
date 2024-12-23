#ifndef WINALLOCATORS_H_
#define WINALLOCATORS_H_

#include <winhandle.h>

#include "allocator.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const mem_allocator *const process_heap_allocator;

winhandle heap_allocator_handle(const mem_allocator *const allocator);

#ifdef __cplusplus
}
#endif

#endif // WIN32_ALLOCATORS_H_
