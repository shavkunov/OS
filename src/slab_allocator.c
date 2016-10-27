#include "slab_allocator.h"
#include "print.h"

void slabFree(struct slabAllocator* allocator, void* addr) {
    struct node* unit = (struct node*)addr;
    unit->next = allocator->head;
    allocator->head = unit;
    allocator->allocatedNodes--;
}

void destroySlab(struct slabAllocator* allocator) {
    buddyFree(allocator);
}

void* slabAlloc(struct slabAllocator* allocator) {
    if (allocator->head == NULL) {
        return NULL;
    }

    struct node* unit = allocator->head;
    allocator->head = unit->next;
    allocator->allocatedNodes++;

    return unit;
}

struct slabAllocator* initSlab(uint64_t dataSize, uint64_t amount) {
    if (amount == 0) {
        return NULL;
    }
  
    if (sizeof(struct node) > dataSize) 
        dataSize = sizeof(struct node);

    uint64_t bytesNeeded = dataSize * amount + sizeof(struct slabAllocator); 
    void* memory = buddyAlloc(bytesNeeded);
    if (memory == NULL) {
        return NULL;
    }
  
<<<<<<< HEAD
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
=======
    struct slabAllocator* allocator = (struct slabAllocator*)memory;

    allocator->next = NULL;
    allocator->prev = NULL;
    allocator->allocatedNodes = 0;
    struct node* unit = allocator->head = (struct node*)((uintptr_t)memory + sizeof(struct slabAllocator));

    for (uint64_t i = 0; i < amount - 1; i++, unit = unit->next) {
        unit->next = (struct node*)((uintptr_t)unit + dataSize);
>>>>>>> parent of 86b94a7... attempt to fix slab
    }
  
    unit->next = NULL;
    return allocator;
}
