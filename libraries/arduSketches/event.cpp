// sketch to drive adxl345 sensor with arduino.
// specs.: http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf, page 18
// tutorial: http://codeyoung.blogspot.it/2009/11/adxl345-accelerometer-breakout-board.html

#include <Wire.h>
#include <Arduino.h>
#include <event.h>

#define DEVICE0 (0x53) // phisyc address for adxl345 - i2c mode - SDO=0 (specs, page 18)
#define DEVICE1 (0x1D) // phisyc address for adxl345 - i2c mode - SDO=1
#define TO_READ (6)
byte buff[TO_READ];

struct event _dataV, *dataV = &_dataV;

void writeTo(int device, byte address, byte val) {
	Wire.beginTransmission(device);	// open i2c communication
	Wire.write(address);	// set register address
	Wire.write(val);	// send value to write
	Wire.endTransmission();	// close communication
}

void readFrom(int device, byte address, int num, byte buff[]) {
	Wire.beginTransmission(device);
	Wire.write(address);	// set register address in write mode 
	Wire.endTransmission();	// close communication in write mode
	Wire.beginTransmission(device);	// re-open communication  for read only access.
	Wire.requestFrom(device, num);	// read 6 bytes, starting from previous address.
	int i = 0;
	while(Wire.available())	// wait for sensor data 
	{
		buff[i] = Wire.read();	// data capture, one byte at once. 
		i++;
	}
	 Wire.endTransmission();
}

void initAdxl345() {
	// power on adxl345 sensors
	writeTo(DEVICE0, 0x2D, 0); // standby.
	writeTo(DEVICE0, 0x2C, 0xC); // clock SCL to 400Hz -> Data rate 800 Hz
	writeTo(DEVICE0, 0x31, 0); // range +/-2g.
	//writeTo(DEVICE0, 0x31, 8); // max resoluzion 4mg/LSB
	//writeTo(DEVICE0, 0x2D, 16); // auto sleep
	writeTo(DEVICE0, 0x2D, 8); // read only mode 

	writeTo(DEVICE1, 0x2D, 0);
	writeTo(DEVICE1, 0x2C, 0xC);
	writeTo(DEVICE1, 0x31, 0);
	//writeTo(DEVICE1, 0x31, 8);
	//writeTo(DEVICE1, 0x2D, 16);
	writeTo(DEVICE1, 0x2D, 8);
}

event *adxl345Driver() {
	int x0,y0,z0,x1,y1,z1;
	int regAddress = 0x32; // first axis acceleration data register - specs: register map, page 23.
	dataV->timeStamp=micros(); // internal arduino current time, starting from boot, in microseconds
	readFrom(DEVICE0, regAddress, TO_READ, buff);
	// 10 precision bits. Needed 2 bytes. Least significant bytes first !
	// each axis reading comes in 10 bit resolution, ie 2 bytes. Least Significant Byte first!!
	// thus we are converting both bytes in to one int and storing them on dataV struct
	x0 = (((int)buff[1]) << 8) | buff[0];
	y0 = (((int)buff[3]) << 8) | buff[2];
	z0 = (((int)buff[5]) << 8) | buff[4];
/*
	dataV->acc0.x = (((int)buff[1]) << 8) | buff[0];
	dataV->acc0.y = (((int)buff[3]) << 8) | buff[2];
	dataV->acc0.z = (((int)buff[5]) << 8) | buff[4];
*/
	readFrom(DEVICE1, regAddress, TO_READ, buff); // same procedure for second device 
	x1 = (((int)buff[1]) << 8) | buff[0];
	y1 = (((int)buff[3]) << 8) | buff[2];
	z1 = (((int)buff[5]) << 8) | buff[4];
/*
	dataV->acc1.x = (((int)buff[1]) << 8) | buff[0];
	dataV->acc1.y = (((int)buff[3]) << 8) | buff[2];
	dataV->acc1.z = (((int)buff[5]) << 8) | buff[4];
*/

	//prepare a string to pass data to main subroutine 
	//sprintf(dataV->str,"%lu %d %d %d %d %d %d\n", dataV->timeStamp, dataV->acc0.x, dataV->acc0.y, dataV->acc0.z, dataV->acc1.x, dataV->acc1.y, dataV->acc1.z);
	sprintf(dataV->str,"%lu %d %d %d %d %d %d\n", dataV->timeStamp, x0, y0, z0, x1, y1, z1);
	//Serial.print(dataV->str);
	return dataV;
}
