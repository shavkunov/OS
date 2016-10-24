#include "timer.h"
#include "ioport.h"
#include "interrupt.h"
#include "print.h"

void initTimer() {
	uint8_t mask = 0;
    //BCD set to zero
    mask |= (2 << 1) | (1 << 4) | (1 << 5); //Periodical mode and init value
    out8(COMMAND_PORT, mask);
    
    //set to min freq
    out8(ZERO_CHANNEL, 0xff);
    out8(ZERO_CHANNEL, 0xff);
    
    //unmask entry master
    setBitMaster(0, 0);
}
