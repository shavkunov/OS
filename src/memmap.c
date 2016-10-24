#include "memmap.h"
#include "print.h"

uint32_t memmapLength = 0;
struct memmapEntry memmap[MEMMAP_MAX_SIZE];

extern uint64_t multiboot_info;
extern char text_phys_begin[];
extern char bss_phys_end[];
extern uint32_t multiboot_header[];

void addEntry(uint64_t addr, uint64_t len, uint32_t t) {
    struct memmapEntry* newEntry = memmap + memmapLength;
    memmapLength++;
    
    newEntry->baseAddr = addr;
    newEntry->length = len;
    newEntry->type = t;
}

void read_memmap(void) {
    printf("Reading Memory Map\n");
    
    //init kernel entry
    struct memmapEntry kernelEntry;
    kernelEntry.baseAddr = (uint64_t)text_phys_begin; 
    kernelEntry.length = (uint64_t)bss_phys_end - (uint64_t)text_phys_begin;
    kernelEntry.type = 0;
    uint64_t kernelLeft = kernelEntry.baseAddr;
    uint64_t kernelRight = kernelEntry.baseAddr + kernelEntry.length - 1;
    memmap[memmapLength] = kernelEntry;
    memmapLength++;

    uint32_t memmapSize = *((uint32_t*) ((char*)multiboot_info + 44));
    char *memmapAddr = *((char**) ((char*)multiboot_info + 48));
    
    while (memmapSize > 0) {
        // current Entry
        uint32_t size = *((uint32_t*) memmapAddr);
        memmapAddr += sizeof(size);
        struct memmapEntry curEntry;
        curEntry.baseAddr = *((uint64_t*) memmapAddr);
        curEntry.length = *((uint64_t*) (memmapAddr + 8));
        curEntry.type = *((uint32_t*) (memmapAddr + 16));
        uint64_t curLeft = curEntry.baseAddr;
        uint64_t curRight = curEntry.baseAddr + curEntry.length - 1;
        memmapSize -= sizeof(size) + size;
        memmapAddr += size;
        
        //intersection with kernel
       
        if (kernelLeft > curRight || kernelRight < curLeft) {
            addEntry(curEntry.baseAddr, curEntry.length, curEntry.type);
            continue;
        }
        
        if (kernelRight < curRight) {
            addEntry(kernelRight + 1, curRight - kernelRight, curEntry.type);
            continue;
        }
        
        if (curLeft < kernelLeft) {
            addEntry(curLeft, kernelLeft - curLeft, curEntry.type);
        }        
    }
            
    printf("End of read memory map\n");
}

void print_memmap(void) {
    printf("Memory Map\n");
    
    for (uint32_t i = 0; i < memmapLength; i++) {
        printf("memory segment %d: 0x%llx - 0x%llx (%d bytes), type %d\n", i, memmap[i].baseAddr, 
               memmap[i].baseAddr + memmap[i].length - 1, memmap[i].length, memmap[i].type);
    }
    
    printf("End of Memory Map\n");
}
