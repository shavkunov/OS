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

void main(void)
{
	qemu_gdb_hang();

	//serial test
	initSerial();
	print("Hello, world!\n");
	//interrupt test
	initInterrupt();
	__asm__ volatile("int $80");
	
	//timer
    initTimer();
    enable_ints();
        
	while (1);
}
