#ifndef WINALLOCATORS_H_
#define WINALLOCATORS_H_

#include <winhandle.h>

#include "allocator.h"

extern const mem_allocator *const process_heap_allocator;

winhandle heap_allocator_handle(const mem_allocator *const allocator);

extern const mem_allocator *const com_allocator;

#define ole_allocator com_allocator

#endif // WIN32_ALLOCATORS_H_
