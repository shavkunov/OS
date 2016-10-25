#ifndef __SLAB_ALLOCATOR_H__
#define __SLAB_ALLOCATOR_H__

#include "stdint.h"
#include "stddef.h"
#include "buddy.h"

struct node {
    struct node* next;
};

struct slabAllocator {
    struct node* head;
    struct slabAllocator *next;
    struct slabAllocator *prev;
    uint64_t allocatedNodes;
};

void slabFree(struct slabAllocator* allocator, void* addr);
void destroySlab(struct slabAllocator* allocator);
void* slabAlloc(struct slabAllocator* allocator);
struct slabAllocator* initSlab(uint64_t dataSize, uint64_t amount);

#endif