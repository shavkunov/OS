#include "paging.h"
#include "print.h"
#include "memmap.h"
#include "memory.h"

extern uint64_t* bootstrap_pml4;
uint64_t* PML4Pages[TABLE_SIZE];
uint64_t fourGB = 4 * ((uint64_t) GB);

void initPaging(void) {
    uint64_t GbAmount = totalMemory / GB + (totalMemory % GB == 0 ? 0 : 1);
    uint64_t PML4Entries = GbAmount / TABLE_SIZE + (GbAmount % TABLE_SIZE == 0 ? 0 : 1); // entries to be mapped

    for (uint32_t i = 0; i < memmapLength; i++) {
        if (memmap[i].type != 1)
            continue;
            
        uint64_t curLeft = memmap[i].baseAddr;
        uint64_t curRight = curLeft + memmap[i].length;
        if (curRight > fourGB)
            curRight = fourGB;
           
        if (curLeft % PAGE_SIZE != 0)
            curLeft += PAGE_SIZE - curLeft % PAGE_SIZE;
        if (curRight <= curLeft)
            continue;
            
        uint64_t pagesForWrite = (curRight - curLeft) / PAGE_SIZE;
        if (pagesForWrite >= PML4Entries) {
            for (uint32_t j = 0; j < PML4Entries; j++) {
                PML4Pages[j + TABLE_SIZE / 2] = (uint64_t*) LOGICAL_ADDR(curLeft); //after hole
                curLeft += PAGE_SIZE;
            }
            
            memmap[memmapLength].type = RESERVED;
            memmap[memmapLength].length = curLeft - memmap[i].baseAddr;
            memmap[memmapLength].baseAddr = memmap[i].baseAddr;
            memmap[i].length -= curLeft - memmap[i].baseAddr;
            memmap[i].baseAddr = curLeft;
            memmapLength++;
            break;
        }
    }
    
    //mapping
    for (uint32_t i = 0; i < GbAmount; i++) {
        uint64_t physAddr = GB * i;
        uint32_t PML4Index = i / TABLE_SIZE + TABLE_SIZE / 2;
  
        PML4Pages[PML4Index][i % TABLE_SIZE] = physAddr | WRITE | PRESENT | PS;
        *((uint64_t*) bootstrap_pml4 + PML4Index) = (uint64_t) PHYSICAL_ADDR(PML4Pages[PML4Index]) | PRESENT | WRITE; 
    }
}
