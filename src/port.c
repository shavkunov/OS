#include <port.h>

int getPort(int number) {
	return 0x3f8 + number;
}

void initSerial() {
	out8(getPort(1), 0); // power off interrupt

	out8(getPort(3), 128); // need to set div to 1
	out8(getPort(0), 1); // set div to 1
	out8(getPort(1), 0);

	out8(getPort(3), 3); // 8 bit frame set
	
}

void print(const char* s) {
	int i = 0;
	while (s[i] != 0) {
		out8(getPort(0), s[i]);
		i++;

		while (!((in8(getPort(5)) >> 5) & 1));
	}		
}
