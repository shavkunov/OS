#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <stdint.h>

#define MASTER_COMMAND_PORT 0x20
#define MASTER_DATA_PORT 0x21

#define SLAVE_COMMAND_PORT 0xA0
#define SLAVE_DATA_PORT 0xA1

struct idtEntryStruct {
	uint16_t offset0;
	uint16_t segmentSelector;
	uint16_t flags;
	uint16_t offset1;
	uint32_t offset2;
	uint32_t reserved;
} __attribute__((packed));

void initInterruptTable();
void initInterruptController();
void initInterrupt();
void setEntryOfIDT(int id, uint64_t e);
void interruptHandler(uint64_t interrupt_id);
void endOfInterrupt(uint8_t port);
void setSlaveMask(uint8_t mask);
void setMasterMask(uint8_t mask);
void setBitMaster(uint8_t id, uint8_t bit);
void setBitSlave(uint8_t id, uint8_t bit);

#endif
