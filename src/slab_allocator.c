#include "slab_allocator.h"
#include "print.h"

void slabFree(struct slabAllocator** allocator, void* addr) {
    struct slabAllocator* newSlab = (struct slabAllocator *) addr;
	newSlab->dataSize = (*allocator)->dataSize;
	newSlab->head = *allocator;
	(*allocator) = newSlab;
}

void destroySlab(struct slabAllocator* allocator) {
    buddyFree(allocator);
}

void* slabAlloc(struct slabAllocator** allocator) {
    if ((*allocator)->head == NULL) {
        struct slabAllocator* newSlab = initSlab((*allocator)->dataSize);
        (*allocator)->head = newSlab;
    }

    void* memory = (void*) (*allocator);
    *allocator = (*allocator)->head;
    return memory;
}

struct slabAllocator* initSlab(uint64_t dataSize) {
    if (sizeof(struct slabAllocator) > dataSize) 
        dataSize = sizeof(struct slabAllocator);
    
    uint64_t bytesNeeded = 0;
    if (8 * dataSize <= PAGE_SIZE) {
        bytesNeeded = PAGE_SIZE;
    } else {
        bytesNeeded = 8 * PAGE_SIZE;
    }
    
    void* memory = buddyAlloc(bytesNeeded);
    if (memory == NULL) {
        return NULL;
    }
  
    struct slabAllocator* allocator = (struct slabAllocator*) memory;

    uint32_t i = 1;
    allocator->dataSize = dataSize;
    struct slabAllocator* unit = allocator;
    while (i * sizeof(allocator) < bytesNeeded) {
        unit->head = (struct slabAllocator *)((uint32_t *)(allocator) + i * dataSize);
		i++;
		unit = unit->head;
		unit->dataSize = dataSize;
		break;
    }
  
    unit->head = NULL;
    return allocator;
}
