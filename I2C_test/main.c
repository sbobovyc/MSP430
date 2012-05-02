// http://e2e.ti.com/support/microcontrollers/msp43016-bit_ultra-low_power_mcus/f/166/t/19570.aspx#76258
//This is the master code.
#include "msp430x22x4.h"
#include "I2C_Driver.h"

unsigned char d = 0;
unsigned char data[10];

void main(void)
{


	InitI2C();
	d = READ_I2C(0x6F,0x10);
	ReadI2CMultipleByte(0x6F,0x10,5,data);
	__no_operation();
	while(1);
}


