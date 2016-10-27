#ifndef __PAGING_H__
#define __PAGING_H__

#include "stdint.h"

#define TABLE_SIZE   512
//PTE FLAGS
#define PRESENT     (1 << 0)
#define WRITE       (1 << 1)
#define PS       (1 << 7)

void initPaging(void);

#endif
