// http://e2e.ti.com/support/microcontrollers/msp43016-bit_ultra-low_power_mcus/f/166/t/19570.aspx#76258
//This is the master code.
#include "msp430x22x4.h"
#include "I2C_Driver.h"

unsigned char d = 0;
unsigned char data[5];

void main(void)
{


	InitI2C();
	d = READ_I2C(0x6F,0x10);
	WRITE_I2C(0x6F, 0x10, 0xFF);
	d = READ_I2C(0x6F,0x10);
	data[0] = 1;
	data[1] = 2;
	data[2] = 3;
	data[3] = 4;
	data[4] = 5;
	WriteI2CMultipleByte(0x6F,0x00,5,data);
	ReadI2CMultipleByte(0x6F,0x00,5,data);
	__no_operation();
	while(1);
}


