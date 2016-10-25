#ifndef __MEMORY_H__
#define __MEMORY_H__

#define VIRTUAL_BASE	0xffffffff80000000
#define MEM_SHIFT 0xffff800000000000
#define PAGE_SIZE	0x1000
#define KERNEL_CS	0x08
#define KERNEL_DS	0x10

#define LOGICAL_ADDR(x) ((x) + MEM_SHIFT)
#define PHYSICAL_ADDR(x) ((x) - MEM_SHIFT)

#endif /*__MEMORY_H__*/
