static void qemu_gdb_hang(void) 
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include "paging.h"
#include "buddy.h"
#include "desc.h"
#include "port.h"
#include "print.h"
#include "interrupt.h"
#include "timer.h"
#include "ints.h"
#include "stdint.h"
#include "memmap.h"
#include "slab_allocator.h"
#include "thread.h"

void init(void) {
    readMemmap();
    printMemmap();
	serial_setup();
	initInterrupt();
	initTimer();
    initPaging();
    initBuddy();
    enable_ints();
    initThreads();
}

void foo(void* arg) {
    for (int i = 0; i < 1000; i++) {
        lock();
        printf("Hello from thread %s with i %d\n", (char*)arg, i);
        unlock();
    }
}


void main(void) {
	qemu_gdb_hang();
	
	init();
    
    uint32_t** a = (uint32_t**)buddyAlloc(sizeof(uint32_t*) * 100);
    
    for (uint32_t i = 0; i < 100; i++) {
        a[i] = (uint32_t*)buddyAlloc(sizeof(uint32_t) * 100);
    }
    
    for (uint32_t i = 0; i < 100; i++) {
        buddyFree(a[i]);
    }
    
    buddyFree((void*) a);
    
    printf("Buddy test successfull\n");
    
    struct slabAllocator* slab = initSlab(4);
    uint32_t* b = (uint32_t*) slabAlloc(&slab);
    
    for (uint32_t i = 0; i < 10; i++) {
        b[i] = i + 10;
    }
    
    slabFree(&slab, b);
    destroySlab(slab);
    printf("Slab test successfull\n");
    
    char arg1[] = "one";
    char arg2[] = "two";
    
    thread* thread1 = threadCreate(foo, &arg1);
    thread* thread2 = threadCreate(foo, &arg2);
    join(thread1);
    join(thread2);
    
    printf("Threads ended\n");   
	while (1);
}
