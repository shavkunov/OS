#ifndef __MEMMAP_H__
#define __MEMMAP_H__

#include <stdint.h>

#define MEMMAP_MAX_SIZE 100

struct memmapEntry {
    uint64_t baseAddr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed));

extern uint32_t memmapLength;
extern struct memmapEntry memmap[];

void addEntry(uint64_t addr, uint64_t len, uint32_t t);
void read_memmap(void);
void print_memmap(void);

#endif
