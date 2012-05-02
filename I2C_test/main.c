// http://e2e.ti.com/support/microcontrollers/msp43016-bit_ultra-low_power_mcus/f/166/t/19570.aspx#76258
//This is the master code.
#include "msp430x22x4.h"
#include "I2C_Driver.h"
#include "RTCC_Driver.h"

unsigned char d = 0;
unsigned char data[5];

RTCCTimeDate		Rtcctimedate;			//Declare a structure
//RTCCTime 			Rtcctime;
//RTCCAlarm 		Rtccalarm;
//RTCCTimeStampVcc 	Rtcctimestampvcc;

void main(void)
{


//	InitI2C();
//	d = READ_I2C(0x6F,0x10);
//	WRITE_I2C(0x6F, 0x10, 0xFF);
//	d = READ_I2C(0x6F,0x10);
//	data[0] = 1;
//	data[1] = 2;
//	data[2] = 3;
//	data[3] = 4;
//	data[4] = 5;
//	WriteI2CMultipleByte(0x6F,0x00,5,data);
//	ReadI2CMultipleByte(0x6F,0x00,5,data);
//	__no_operation();
//	while(1);


	unsigned char Cnt;
	unsigned char Buffer[64];
	InitRTCC();								//Initialize MCP79410,Produce 1Hz square wave on MFP
	Rtcctimedate.Day = 6;					//Write Monday as a day to RTCC
	Rtcctimedate.Date = 0x31;				//Write Date as 1 to RTCC
	Rtcctimedate.Month = 0x12;				//Write 0x11 as month to RTCC
	Rtcctimedate.Year = 0x11;				//Write 0x11 as year to RTCC
	Rtcctimedate.Hour =0x23;				//Write 0x23 as hour to RTCC
	Rtcctimedate.Min = 0x59;				//Write 0x59 as minute to RTCC
	Rtcctimedate.Sec = 0x59;				//Write 0x59 as second to RTCC

	WriteRTCCTimeDate(&Rtcctimedate);		//Update RTCC with new time stamp stored in Rtcctimedate

	for(Cnt = 0; Cnt < 64; Cnt++)
	{
		Buffer[Cnt] = Cnt;
	}
	WriteSRAMBlock(&Buffer[0]);				//Write SRAM with values stored in Buffer variable
	__no_operation();
	__no_operation();
	__no_operation();
	for(Cnt = 0; Cnt < 64; Cnt++)
	{
		Buffer[Cnt] = 0;					//Clear Buffer
	}
	__no_operation();
	__no_operation();
	__no_operation();
	ReadSRAMBlock(&Buffer[0]);				//Read SRAM in Buffer
	while(1)
	{
		__no_operation();
		__no_operation();
		__no_operation();
		ReadRTCCTimeDate(&Rtcctimedate);

	}//while
}


