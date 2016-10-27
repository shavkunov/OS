#include "buddy.h"
#include "print.h"

uint32_t minPowerOfTwo(uint64_t x) {
    uint32_t res = 0;
    uint64_t pow = 1;
    while (pow < x) {
        res++;
        pow *= 2;
    }
    return res;
}

uint64_t getIndexOfDescriptor(struct descriptor* desc) {
    return desc - desc->info->descriptors;
}

struct descriptor* getBuddy(struct descriptor* desc) {
    uint64_t descIndex = getIndexOfDescriptor(desc);
    uint64_t buddyIndex = descIndex ^ (1 << desc->level); 
    if (buddyIndex >= desc->info->size)
        return NULL;

    struct descriptor* buddyDesc = desc->info->descriptors + buddyIndex;
    
    if (buddyDesc->level != desc->level || buddyDesc->allocated == 1)
        return NULL;
        
    return buddyDesc;
}

void* getStartAddr(struct descriptor* desc) {
    return (void*)(PAGE_SIZE * getIndexOfDescriptor(desc) + desc->info->startAddr); 
}

struct descriptor* getDescriptor(void* searchAddr) {
    uint64_t addr = PHYSICAL_ADDR((uint64_t) searchAddr);
    for (uint64_t i = 0; i < memmapLength; i++) {
        struct memmapEntry* curEntry = memmap + i; 
        if ((curEntry->baseAddr <= addr) && (addr < curEntry->baseAddr + curEntry->length))
            return metaInfo[i]->descriptors + (addr - PHYSICAL_ADDR(metaInfo[i]->startAddr)) / PAGE_SIZE;
    }
    return NULL;
}

void* allocLevel(uint32_t level) { 
    for (uint32_t i = level; i < MAX_LEVEL; i++) {
        if (descHeads[i] == NULL)
            continue;

        struct descriptor* desc = descHeads[i];
        while (desc->level > level) 
            desc = split(desc);

        eraseHead(desc);
        desc->allocated = 1;
        return getStartAddr(desc);
    }
    return NULL;
}

void* buddyAlloc(uint64_t size) {
    uint64_t pages = (PAGE_SIZE - 1 + size) / PAGE_SIZE;
    return allocLevel(minPowerOfTwo(pages));
}

void eraseHead(struct descriptor* desc) { 
    if (desc->left != NULL)
        desc->left->right = desc->right;
    if (desc->right != NULL)
        desc->right->left = desc->left;
    if (desc->left == NULL)
        descHeads[desc->level] = desc->right;

    desc->left = NULL;
    desc->right = NULL;
}

void insertHead(struct descriptor* desc) {
    desc->right = descHeads[desc->level];
    descHeads[desc->level] = desc;
    if (desc->right != NULL)
        desc->right->left = desc;
}

struct descriptor* merge(struct descriptor* desc) {
    if (desc->allocated == 1)
        return NULL;

    struct descriptor* buddyDesc = getBuddy(desc);
    if (buddyDesc == NULL)
        return NULL;

    eraseHead(desc);
    eraseHead(buddyDesc);

    if (buddyDesc < desc)
        desc = buddyDesc;

    desc->level++;
    insertHead(desc);
    return desc;
}

struct descriptor* split(struct descriptor* desc) {
    if (desc->allocated == 1 || desc->level == 0)
        return NULL;
  
    eraseHead(desc);
    desc->level--;
    insertHead(desc);
    insertHead(getBuddy(desc));
    return desc;
}

void buddyFree(void* addr) {
    struct descriptor* desc = getDescriptor(addr);

    desc->allocated = 0;
    insertHead(desc);
    while ((desc = merge(desc)));
}

void addSegmentMemory(struct memmapEntry* segment, uint64_t segmentIndex) {
    uint64_t firstAddr = segment->baseAddr;
    uint64_t fourGB = 4 * ((uint64_t) GB);
    
    if (firstAddr > fourGB) {
        return;
    }
    
    if (firstAddr + segment->length > fourGB) {
        segment->length = fourGB - firstAddr;
    }
    
    if (firstAddr % PAGE_SIZE != 0) {
        firstAddr += PAGE_SIZE - (firstAddr % PAGE_SIZE);
    }
  
    uint64_t segmentSize = segment->length - (firstAddr - segment->baseAddr);
    uint64_t pagesAmount = (segmentSize - sizeof(struct reservedTreeMetaInfo)) / 
                                          (PAGE_SIZE + sizeof(struct descriptor));

    if (pagesAmount < 1) {
        return;
    }
  
    firstAddr = LOGICAL_ADDR(firstAddr);
    struct descriptor* firstDesc = (struct descriptor*)(firstAddr + PAGE_SIZE * pagesAmount);
    struct reservedTreeMetaInfo* info = (struct reservedTreeMetaInfo*)(firstDesc + pagesAmount);
    
    info->size = pagesAmount;
    info->descriptors = firstDesc;
    info->startAddr = firstAddr;
    
    for (uint64_t i = 0; i < info->size; i++) {
        info->descriptors[i].allocated = 0;
        info->descriptors[i].level = 0;
        info->descriptors[i].info = info;
        info->descriptors[i].right = NULL;
        info->descriptors[i].left = NULL;
        insertHead(info->descriptors + i);
    }
 
    metaInfo[segmentIndex] = info;
}

void initHeads(void) {
    for (uint64_t i = 0; i < MAX_LEVEL; i++) {
        struct descriptor* head = NULL;
        while (descHeads[i]) {
            if (merge(descHeads[i]) == NULL) {
                struct descriptor* desc = descHeads[i];
                eraseHead(desc);
                desc->right = head;
                if (head) {
                    head->left = desc;
                }
                head = desc;
            }
        }

        descHeads[i] = head;
    }
}

void initBuddy(void) {
    printf("Begin of Buddy Allocator Initialization\n");

    for (uint64_t i = 0; i < memmapLength; i++) {
        struct memmapEntry* block = memmap + i;
        if (block->type == RESERVED) {
            continue;
        }

        addSegmentMemory(block, i);
    }
  
    printf("added all memory segments\n");
    initHeads();
  
    printf("End of Buddy Allocator Initialization\n");
}
