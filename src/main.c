static void qemu_gdb_hang(void) 
{
#ifdef DEBUG
	static volatile int wait = 1;

	while (wait);
#endif
}

#include "desc.h"
#include "port.h"
#include "interrupt.h"
#include "timer.h"
#include "ints.h"
#include <stdint.h>
#include <memmap.h>

void main(void) {
	qemu_gdb_hang();

    read_memmap();

	//serial test
	serial_setup();
	print_memmap();
	//interrupt test
	initInterrupt();
	//__asm__ volatile("int $80");
	
	//timer
    //initTimer();
    
    enable_ints();
        
	//while (1);
}
