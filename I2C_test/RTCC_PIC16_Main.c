/******************************************************************************************************************
File Name    			: RTCC_PIC16_Main.c 
RTCC 					: MC79410, 
Language Tool Suites 	: PIC16 family, C language. 
Dependencies 			: htc.h
Processor   			: PIC16F1937
Hardware     			: PIC18 EXPLORER(DM183032), MCP79410  RTCC PICtail Plus Daughter Card(AC164140)
I.D.E.       			: MPLAB v8.80
Compiler				: Hi-Tech PRO C V9.83
Company     			: Microchip Technology Inc. 
/*******************************************************************************************************************

                  SOFTWARE  LICENSE AGREEMENT    

"Microchip Technology Inc." ("Microchip") licenses this software to you 
solely for use with Microchip's Serial RTCC products.  
The software is owned by Microchip and/or its licensors, and is protected 
under applicable copyright laws.  All rights reserved.
SOFTWARE IS PROVIDED "AS IS."  MICROCHIP AND ITS LICENSOR EXPRESSLY 
DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING 
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
FOR A PARTICULAR PURPOSE,OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP 
AND ITS LICENSORS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, 
COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY 
CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS."
/******************************************************************************************************************
Firmware Description : 
This firmware assumes 10Mhz as system clock to PIC16F1937 and uses MSSP module in I2C master mode to communicate with MCP7941x.
For further details on firmware please refer AN1414. The firmware does not enable code protection for flash and data memory. 
It is the responsibility of the user to enable required code protection. Hi Tech PRO V9.83 is used with Optimization selected as Global,Assembler, Speed with 9 as highest possible optimization.
*******************************************************************************************************************/
/******************************************************************************************************************
Firmware Revision History
Date					Author 			Revision			Changes
March,10,2011			PM				1.0				Initial Release
*******************************************************************************************************************/

#include<htc.h>
#include "..\Header\RTCC_Driver.h"

/*Use HS oscilator mode for 10Mhz Crystal
  Disable Watch Dog Timer
  Turn off Power up timer
  Use MCLR as Reset pin
  Disable Code protection
  Disable Code protection for EEPROM
  Disable BOR
  Turn off internal osc. to external osc.switch over
  Turn off Fail Safe Clock Monitor
  Do not use Vcap capacitor on Vcap pins
  Turn off PLL
  Turn off Low Voltage Programming
*/
__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(VCAPEN_OFF & PLLEN_OFF & LVP_OFF);

RTCCTimeDate		Rtcctimedate;			//Declare a structure
//RTCCTime 			Rtcctime;
//RTCCAlarm 		Rtccalarm;
//RTCCTimeStampVcc 	Rtcctimestampvcc;

void main(void)
{
	/*To maintain text formating use Tab as 4 in MPLAB IDE V8.80.Right Click - Properties --> 'C' File Types */
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
	NOP();
	NOP();
	NOP();
	for(Cnt = 0; Cnt < 64; Cnt++)	
	{
		Buffer[Cnt] = 0;					//Clear Buffer
	}
	NOP();
	NOP();
	NOP();
	ReadSRAMBlock(&Buffer[0]);				//Read SRAM in Buffer
	while(1)
	{
		NOP();
		NOP();
		NOP();
		ReadRTCCTimeDate(&Rtcctimedate);
		
	}//while

}//main
