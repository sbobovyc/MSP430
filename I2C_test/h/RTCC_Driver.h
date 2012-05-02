/******************************************************************************************************************
File Name    			: RTCC_Driver.h 
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
#ifndef	__RTCC_DRIVER_H
#define	__RTCC_DRIVER_H

#include "I2C_Driver.h"

typedef struct{
	unsigned char Day;
	unsigned char Sec;
	unsigned char Min;
	unsigned char Hour;
	unsigned char Date;
	unsigned char Month;
	unsigned char Year;
}RTCCTimeDate;

typedef struct{
	unsigned char Sec;
	unsigned char Min;
	unsigned char Hour;
}RTCCTime;

typedef struct{
	unsigned char Day;
	unsigned char Date;
	unsigned char Month;
	unsigned char Year;
}RTCCDate;

typedef struct{
	
	union{
		unsigned char DVal;	//This will be used when Alarm is to be read
		struct{
			unsigned Day : 3;
			unsigned ALMIF : 1;
			unsigned ALMConfig : 3;
			unsigned ALMPOL : 1;
		};
	}DummyVal;
	unsigned char Sec;
	unsigned char Min;
	unsigned char Hour;
	unsigned char Date;
	unsigned char Month;
}RTCCAlarm;

typedef struct{
	union{
		unsigned char DM;
		struct{
			unsigned Month : 5;
			unsigned Day : 3;
		};
	}Day_Month;
	unsigned char Min;
	unsigned char Hour;
	unsigned char Date;
}RTCCTimeStampVcc;

#define		CNTRL_BYTE_SRAM_RTCC					0x6F
#define		CNTRL_BYTE_EEPROM_RTCC					0x57
#define		SRAM_BLOCK_SIZE							64
#define		SRAM_BLOCK_START_ADD					0x20
#define		EEPROM_PAGE_SIZE						8
#define		UNIQUE_ID_START_ADD						0xF0

#define		RTCC_SEC_REG							0x00
#define		RTCC_MIN_REG							0x01
#define		RTCC_HOUR_REG							0x02
#define		RTCC_DAY_REG							0x03
#define		RTCC_DATE_REG							0x04
#define		RTCC_MONTH_REG							0x05
#define		RTCC_YEAR_REG							0x06
#define		RTCC_CNTRL_REG							0x07
#define		RTCC_CAL_REG							0x08
#define		RTCC_UNLOCK_REG							0x09
#define		RTCC_ALARM0_SEC_REG						0x0A
#define		RTCC_ALARM0_MIN_REG						0x0B
#define		RTCC_ALARM0_HOUR_REG					0x0C
#define		RTCC_ALARM0_DAY_REG						0x0D
#define		RTCC_ALARM0_DATE_REG					0x0E
#define		RTCC_ALARM0_MONTH_REG					0x0F
#define		RTCC_ALARM1_SEC_REG						0x11	
#define		RTCC_ALARM1_MIN_REG						0x12
#define		RTCC_ALARM1_HOUR_REG					0x13
#define		RTCC_ALARM1_DAY_REG						0x14
#define		RTCC_ALARM1_DATE_REG					0x15
#define		RTCC_ALARM1_MONTH_REG					0x16
#define		RTCC_VCC_FAIL_MIN_REG					0x18
#define		RTCC_VCC_FAIL_HOUR_REG					0x19
#define		RTCC_VCC_FAIL_DATE_REG					0x1A
#define		RTCC_VCC_FAIL_DAY_MONTH_REG				0x1B
#define		RTCC_VCC_RESTORE_MIN_REG				0x1C
#define		RTCC_VCC_RESTORE_HOUR_REG				0x1D
#define		RTCC_VCC_RESTORE_DATE_REG				0x1E
#define		RTCC_VCC_RESTORE_DAY_MONTH_REG			0x1F

#define		RTCC_EN_OSC_START						0x80
#define		RTCC_EN_12_24							0x40
#define		RTCC_EN_VBATEN							0x08
#define		RTCC_EN_VBAT							0x10
#define		RTCC_EN_OSCON							0x20
#define		RTCC_EN_LP								0x20
#define		RTCC_EN_RS0								0x01
#define		RTCC_EN_RS1								0x02
#define		RTCC_EN_RS2								0x04
#define		RTCC_EN_EXTOSC							0x08
#define		RTCC_EN_ALM0							0x10
#define		RTCC_EN_ALM1							0x20
#define		RTCC_EN_SQWE							0x40
#define		RTCC_EN_OUT								0x80

#define		RTCC_EN_ALARM_SEC_MATCH					0x00
#define		RTCC_EN_ALARM_MIN_MATCH					0x10
#define		RTCC_EN_ALARM_HOUR_MATCH				0x20
#define		RTCC_EN_ALARM_DAY_MATCH					0x30
#define		RTCC_EN_ALARM_DATE_MATCH				0x40
#define		RTCC_EN_ALARM_TIMESTAMP_MATCH			0x70

#define		RTCC_MFP_1HZ							0x00
#define		RTCC_MFP_4096HZ							0x01
#define		RTCC_MFP_8192HZ							0x02
#define		RTCC_MFP_32768HZ						0x03
#define		RTCC_CAL_ENABLE							0x10;	

/*APIs for RTCC basic functionalities*/
void InitRTCC(void);
void ReadRTCCTimeDate(RTCCTimeDate* ptrToUserVal);
void WriteRTCCTimeDate(RTCCTimeDate* ptrToUserVal);
void ReadRTCCTime(RTCCTime* ptrToUserVal);
void WriteRTCCTime(RTCCTime* ptrToUserVal);
void ReadRTCCDate(RTCCDate* ptrToUserVal);
void WriteRTCCDate(RTCCDate* ptrToUserVal);
void ReadRTCCAlarm0(RTCCAlarm* ptrToUserVal);
void WriteRTCCAlarm0(RTCCAlarm* ptrToUserVal);
void ReadRTCCAlarm1(RTCCAlarm* ptrToUserVal);
void WriteRTCCAlarm1(RTCCAlarm* ptrToUserVal);
bit ReadRTCCAlarm0Flag(void);
bit ReadRTCCAlarm1Flag(void);
void ReadRTCCTimeStamp_VCC_Fail(RTCCTimeStampVcc* ptrToUserVal);
void ReadRTCCTimeStamp_VCC_Restore(RTCCTimeStampVcc* ptrToUserVal);
void ClearRTCCTimeStamp(void);
void RTCC_OSC_OFF(void);
char ReadRTCC_Cal_Reg(void);
void WriteRTCC_Cal_Reg(char Cal_Val);

/*APIs for on chip SRAM*/
unsigned char ReadSRAMByte(unsigned char Add_Val);
void WriteSRAMByte(unsigned char Add_Val,unsigned char Data_Val);
void ReadSRAMBlock(unsigned char *ptrtoVal);
void WriteSRAMBlock(unsigned char *ptrtoVal);

/*APIs for on chip EEPROM*/
void WriteEEPROMByte(unsigned char Add_Val,unsigned char Data_Val);
unsigned char ReadEEPROMByte(unsigned char Add_Val);
void WriteEEPROMPage(unsigned char *ptrtoVal, unsigned char Add_Val);
void ReadEEPROMPage(unsigned char *ptrtoVal,unsigned Add_Val);
void ReadEEPROMArray(unsigned char *ptrtoVal,unsigned Add_Val, unsigned char Num_Data);
bit EEPROMBusy(void);

/*APIs for on chip Unique ID*/
void WriteUniqueID(unsigned char *ptrtoVal);
void ReadUniqueID(unsigned char *ptrtoVal);

#endif

