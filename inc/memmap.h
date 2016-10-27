#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#include <stdint.h>

#define MEMMAP_MAX_SIZE 100
#define RESERVED 2

struct memmapEntry {
    uint64_t baseAddr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));

extern uint32_t memmapLength;
extern struct memmapEntry memmap[];
uint64_t totalMemory;

void addEntry(uint64_t addr, uint64_t len, uint32_t t);
void readMemmap(void);
void printMemmap(void);

#endif
