#ifndef __SLAB_ALLOCATOR_H__
#define __SLAB_ALLOCATOR_H__

#include "stdint.h"
#include "stddef.h"
#include "buddy.h"


struct slabAllocator {
    struct slabAllocator* head;
    uint64_t dataSize;
};

void slabFree(struct slabAllocator** allocator, void* addr);
void destroySlab(struct slabAllocator* allocator);
void* slabAlloc(struct slabAllocator** allocator);
struct slabAllocator* initSlab(uint64_t dataSize);

#endif
