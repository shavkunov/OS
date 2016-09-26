#ifndef __TIMER_H__
#define __TIMER_H__

#define COMMAND_PORT 0x43
#define ZERO_CHANNEL 0x40

#include "interrupt.h"
#include <stdint.h>

void initTimer();

#endif
