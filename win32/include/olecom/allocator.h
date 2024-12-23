#ifndef OLE_ALLOCATOR_H
#define OLE_ALLOCATOR_H


#ifdef __cplusplus
extern "C" {
#endif


extern const mem_allocator *const com_allocator;

#define ole_allocator com_allocator


#ifdef __cplusplus
}
#endif


#endif // OLE_ALLOCATOR_H
