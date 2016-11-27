#include "interrupt.h"
#include "memory.h"
#include "desc.h"
#include "ints.h"
#include "ioport.h"
#include "print.h"
#include "thread.h"

static struct idtEntryStruct idt[100];
extern uint64_t arrayHandlers[];
uint8_t curMasterMask = 0;
uint8_t curSlaveMask = 0;

void setEntryOfIDT(int id, uint64_t e) {
    idt[id].offset0 = e & 0xffff;
	idt[id].segmentSelector = KERNEL_CS;
	idt[id].flags = (1 << 15) | (14 << 8); // Bit P and INTERRUPT GATE
    idt[id].offset1 = (e >> 16) & 0xffff;
    idt[id].offset2 = (e >> 32) & 0xffffffff;
    idt[id].reserved = 0;
}

void initInterruptTable() {
	for (int i = 0; i < 100; i++)
		setEntryOfIDT(i, arrayHandlers[i]);

	struct desc_table_ptr idtr;
	idtr.addr = (uint64_t) idt;
	idtr.size = sizeof(idt) - 1;
	write_idtr(&idtr);
}

void setMasterMask(uint8_t mask) {
	curMasterMask = mask;
	out8(MASTER_DATA_PORT, mask);
}

void setSlaveMask(uint8_t mask) {
	curSlaveMask = mask;
	out8(SLAVE_DATA_PORT, mask);
}

void setBitMaster(uint8_t id, uint8_t bit) {
    if (((curMasterMask >> id) & 1) != bit) {
        if (bit == 0) {
            curMasterMask ^= (1 << id);
        } else {
            curMasterMask |= (1 << id);
        }
    }  
    setMasterMask(curMasterMask);
}

void setBitSlave(uint8_t id, uint8_t bit) {
    if (((curSlaveMask >> id) & 1) != bit) {
        if (bit == 0) {
            curSlaveMask ^= (1 << id);
        } else {
            curSlaveMask |= (1 << id);
        }
    }
    setSlaveMask(curSlaveMask);
}

void initInterruptController() {
	//Init master controller
    out8(MASTER_COMMAND_PORT, (1 << 0) | (1 << 4));
    out8(MASTER_DATA_PORT, 32);
    out8(MASTER_DATA_PORT, (1 << 2));
    out8(MASTER_DATA_PORT, (1 << 0));
    
	//Init slave controller
    out8(SLAVE_COMMAND_PORT, (1 << 0) | (1 << 4));
    out8(SLAVE_DATA_PORT, 40);
    out8(SLAVE_DATA_PORT, 2);
    out8(SLAVE_DATA_PORT, (1 << 0));
	
	setMasterMask(0xff ^ (1 << 2));
	setSlaveMask(0xff);
}

void initInterrupt() {
    initInterruptTable();
    initInterruptController();
}

void endOfInterrupt(uint8_t port) {
	out8(port, (1 << 5));
}

void interruptHandler(uint64_t interruptIndex) { 
    // Master End of Interrupt
    if (32 <= interruptIndex && interruptIndex <= 39) {
        if (interruptIndex == 32) {
           callSwitcher();
        }
    
        endOfInterrupt(MASTER_COMMAND_PORT);
    }

    // Slave End Of Interrupt
    if (40 <= interruptIndex && interruptIndex <= 47) {
        endOfInterrupt(MASTER_COMMAND_PORT);
        endOfInterrupt(SLAVE_COMMAND_PORT);
    }
}
 
