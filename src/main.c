static void qemu_gdb_hang(void) 
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include "buddy.h"
#include "desc.h"
#include "port.h"
#include "print.h"
#include "interrupt.h"
#include "timer.h"
#include "ints.h"
#include "stdint.h"
#include "memmap.h"

void init(void) {
    readMemmap();
	serial_setup();
	initInterrupt();
    initTimer();
    initBuddy();
    enable_ints();
}

void main(void) {
	qemu_gdb_hang();
	
	init();
    printMemmap();
    
    uint32_t** a = (uint32_t**)buddyAlloc(sizeof(uint32_t*) * 100);
    
    for (uint32_t i = 0; i < 100; i++) {
        a[i] = (uint32_t*)buddyAlloc(sizeof(uint32_t) * 100);
    }
    
    buddyFree((void*) a);
    
    printf("Test successfull\n");
	while (1);
}
