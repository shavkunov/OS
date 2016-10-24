#ifndef __PORT_H__
#define __PORT_H__

#include <stddef.h>
#include <ioport.h>

void serial_setup(void);
void serial_putchar(int c);
void serial_write(const char *buf, size_t size);

#endif /*__PORT_H__*/

