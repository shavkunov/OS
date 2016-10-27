#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <stdint.h>
#include <stddef.h>
#include <memory.h>
#include <memmap.h>

#define MAX_LEVEL 32

struct descriptor;

struct reservedTreeMetaInfo {
    uint64_t size;
    struct descriptor* descriptors;
    uint64_t startAddr; // LOGICAL
};

struct descriptor {
    uint8_t allocated;
    uint8_t level;
    struct reservedTreeMetaInfo* info;
    struct descriptor* left;
    struct descriptor* right;
};

uint64_t getIndexOfDescriptor(struct descriptor* desc);
struct descriptor* getBuddy(struct descriptor* desc);
struct descriptor* getDescriptor(void* addr);
void eraseHead(struct descriptor* desc);
void insertHead(struct descriptor* desc);
struct descriptor* merge(struct descriptor* desc);
struct descriptor* split(struct descriptor* desc);
void addSegmentMemory(struct memmapEntry* segment, uint64_t segmentIndex);
void initHeads(void);
void initBuddy(void);
void* allocLevel(uint32_t order);
void* buddyAlloc(uint64_t size);
void  buddyFree(void* addr);
uint32_t minPowerOfTwo(uint64_t x);

#endif
