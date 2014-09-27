#include <Arduino.h>

#define DELAY 10

struct acc {
	int x,y,z;
};

struct event {
	unsigned long timeStamp;
	struct acc acc0,acc1;
	char str[50]; // pay attention: arduino memory is small!!!
};

struct event *adxl345Driver();

void readFrom(int device, byte address, int num, byte buff[]);
void writeTo(int device, byte address, byte val);
void initAdxl345();
