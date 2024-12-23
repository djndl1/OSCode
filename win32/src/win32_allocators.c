#include "winallocator.h"
#include "winhandle.h"

#include <objbase.h>

static HANDLE get_allocator_heap(const mem_allocator *const self)
{
    return self->user_data == nullptr
        ? GetProcessHeap() : (HANDLE)self->user_data;
}

winhandle heap_allocator_handle(const mem_allocator *const self)
{
    HANDLE handle = get_allocator_heap(self);
    return (winhandle) { .handle = handle };
}


static mem_alloc_result allocate_from_heap(const mem_allocator *const self,
                                           size_t byte_count)
{
    HANDLE heapHandle = get_allocator_heap(self);
    void *mem = HeapAlloc(heapHandle, 0, byte_count);
    mem_alloc_result result = { 0 };
    result.mem = mem;
    if (result.mem == nullptr) {
        result.error = GetLastError();
    }
    return result;
}

static void free_from_heap(const mem_allocator * const self,
                           void *mem)
{
    HANDLE heapHandle = get_allocator_heap(self);
    HeapFree(heapHandle, 0, mem);
}

static mem_alloc_result reallocate_from_heap(const mem_allocator *const self,
                                             void *mem,
                                             size_t newsize)
{
    HANDLE heapHandle = get_allocator_heap(self);
    void *newmem = HeapReAlloc(heapHandle, 0, mem, newsize);
    mem_alloc_result result = { 0 };
    result.mem = newmem;
    if (result.mem == nullptr) {
        result.error = GetLastError();
    }
    return result;
}



static const mem_allocator _win32_heap_allocator_table = {
    .user_data = nullptr,
    .allocate = allocate_from_heap,
    .deallocate = free_from_heap,
    .reallocate = reallocate_from_heap,
};

const mem_allocator *const process_heap_allocator = &_win32_heap_allocator_table;

static mem_alloc_result com_allocate(const mem_allocator *const self, size_t count)
{
    void *mem = CoTaskMemAlloc(count);
    mem_alloc_result result = { 0 };
    result.mem = mem;
    if (result.mem == nullptr) {
        result.error = 1;
    }
    return result;
}

static mem_alloc_result com_reallocate(const mem_allocator *const self,
                                       void *mem,
                                       size_t count)
{
    void *newmem = CoTaskMemRealloc(mem, count);
    mem_alloc_result result = { 0 };
    result.mem = newmem;
    if (newmem == nullptr) {
        result.error = 1;
    }
    return result;
}

static void com_deallocate(const mem_allocator *const self, void *mem)
{
    CoTaskMemFree(mem);
}

static const mem_allocator _com_allocator_table = {
    .user_data = nullptr,
    .allocate = com_allocate,
    .deallocate = com_deallocate,
    .reallocate = com_reallocate,
};

const mem_allocator *const com_heap_allocator = &_com_allocator_table;
