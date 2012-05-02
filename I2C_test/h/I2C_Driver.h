/******************************************************************************************************************
File Name    			: I2C_Driver.h 
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
It is the responsibility of the user to enable required code protection.
*******************************************************************************************************************/
/******************************************************************************************************************
Firmware Revision History
Date					Author 			Revision			Changes
March,10,2011			PM				1.0				Initial Release
*******************************************************************************************************************/
#ifndef	__I2C_DRIVER_H
#define	__I2C_DRIVER_H


#define         GetSystemClock()        10000000UL                  //PIC Operating Frequency in Hz
#define         GetPeripheralClock()    (GetSystemClock() / 4)
#define         GetInstructionClock()   (GetSystemClock() / 4)
#define         I2C_CLOCK               100000UL                    //Enter I2C clock in Hz
#define         I2C_SSPADD_VAL          ((GetInstructionClock()/I2C_CLOCK) - 1)

void InitI2C(void);
void WRITE_I2C(unsigned char Slave_Add,unsigned char Add, unsigned char Val);
unsigned char READ_I2C(unsigned char Slave_Add,unsigned char Add);
void WriteI2CMultipleByte(unsigned char Slave_Add,unsigned char Add,unsigned char Cnt_Byte, unsigned char *ptrToData);
void ReadI2CMultipleByte(unsigned char Slave_Add,unsigned char Add,unsigned char Cnt_Byte, unsigned char *ptrToData);

#endif
