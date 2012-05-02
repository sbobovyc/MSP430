/******************************************************************************************************************
File Name    			: RTCC_Driver.c 
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

#include "RTCC_Driver.h"

/********************************************************************
 * Function:        void InitRTCC(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Initialize I2C module of PIC and configure MCP7941x.
 *					This function must be called before calling any
 *					of the function available
 *******************************************************************/
void InitRTCC(void)
{
	unsigned char Temp = 0;
	InitI2C();
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC, RTCC_CNTRL_REG, \
	RTCC_EN_SQWE | \
	RTCC_MFP_1HZ );
	Temp = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_SEC_REG);
	if(!(Temp & 0x80))
	{
		//Oscilator bit of RTCC is off. Turn on to start RTCC function
		WRITE_I2C(CNTRL_BYTE_SRAM_RTCC, RTCC_SEC_REG, RTCC_EN_OSC_START);
	}
}

/********************************************************************
 * Function:        void ReadRTCCTimeDate(RTCCTimeDate* ptrToUserVal)
 *
 * PreCondition:    RTCCTimeDate structure must have been declared.
 *
 * Input:           Address of typedefined RTCCTimeDate structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update typedefined RTCCTimeDate structure with Time
 *					and Date read from MCP7941x
 *******************************************************************/
void ReadRTCCTimeDate(RTCCTimeDate* ptrToUserVal)
{
	ptrToUserVal-> Sec = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_SEC_REG) & 0x7F); 
	ptrToUserVal-> Min = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MIN_REG);
	ptrToUserVal-> Hour = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_HOUR_REG) & 0x7F);
	
	ptrToUserVal-> Day = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DAY_REG) & 0x07);
	ptrToUserVal-> Date = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DATE_REG);
	ptrToUserVal-> Month = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MONTH_REG) & 0x1F);
	ptrToUserVal-> Year = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_YEAR_REG);

}

/********************************************************************
 * Function:        void WriteRTCCTimeDate(RTCCTimeDate* ptrToUserVal)
 *
 * PreCondition:    RTCCTimeDate structure must have been declared.
 *
 * Input:           Address of typedefined RTCCTimeDate structure
 *
 * Output:          None
 *
 * Side Effects:    It is firmware responsibility to load correct format of Hour register.
 *					If 24hour mode is selected while initializing MCP7941x then hour register
 *					should be loaded accordingly
 *
 * Overview:        Update MCP7941x Time & Date registers with the value
 *					provided in typedefined RTCCTimeDate structure 
 *******************************************************************/
void WriteRTCCTimeDate(RTCCTimeDate* ptrToUserVal)
{
	unsigned char Temp = 0;

	ptrToUserVal-> Sec = ptrToUserVal-> Sec | 0x80;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_SEC_REG, ptrToUserVal-> Sec) ;

	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MIN_REG,ptrToUserVal-> Min);

	Temp = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_HOUR_REG);
	Temp = Temp & 0x40;
	ptrToUserVal-> Hour = ptrToUserVal-> Hour | Temp;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_HOUR_REG,ptrToUserVal-> Hour);
	
	Temp = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DAY_REG);
	Temp = Temp & 0x38;
	ptrToUserVal-> Day = ptrToUserVal-> Day | Temp;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DAY_REG,ptrToUserVal-> Day);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DATE_REG, ptrToUserVal-> Date) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MONTH_REG, ptrToUserVal-> Month) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_YEAR_REG, ptrToUserVal-> Year) ;
}

/********************************************************************
 * Function:        void ReadRTCCTime(RTCCTime* ptrToUserVal)
 *
 * PreCondition:    RTCCTime structure must have been declared.
 *
 * Input:           Address of typedefined RTCCTime structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update typedefined RTCCTime structure with Time
 *					read from MCP7941x 
 *******************************************************************/
void ReadRTCCTime(RTCCTime* ptrToUserVal)
{
	ptrToUserVal-> Sec = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_SEC_REG) & 0x7F); 
	ptrToUserVal-> Min = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MIN_REG);
	ptrToUserVal-> Hour = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_HOUR_REG) & 0x7F);

}

/********************************************************************
 * Function:        void WriteRTCCTime(RTCCTime* ptrToUserVal)
 *
 * PreCondition:    RTCCTime structure must have been declared.
 *
 * Input:           Address of typedefined RTCCTime structure
 *
 * Output:          None
 *
 * Side Effects:  	It is firmware responsibility to load correct format in Hour registers.
 *					If 24hour mode is selected while initializing MCP7941x then hour register
 *					should be loaded accordingly
 * Overview:        Update MCP7941x Time registers with the value
 *					provided in typedefined RTCCTime structure 
 *******************************************************************/
void WriteRTCCTime(RTCCTime* ptrToUserVal)
{
	unsigned char Temp = 0;
	ptrToUserVal-> Sec = ptrToUserVal-> Sec | 0x80;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_SEC_REG, ptrToUserVal-> Sec) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MIN_REG,ptrToUserVal-> Min);
	Temp = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_HOUR_REG);
	Temp = Temp & 0x40;
	ptrToUserVal-> Hour = ptrToUserVal-> Hour | Temp;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_HOUR_REG,ptrToUserVal-> Hour);
}

/********************************************************************
 * Function:        void ReadRTCCDate(RTCCDate* ptrToUserVal)
 *
 * PreCondition:    RTCCDate structure must have been declared.
 *
 * Input:           Address of typedefined RTCCTime structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update typedefined RTCCDate structure with Date
 *					read from MCP7941x 
 *******************************************************************/
void ReadRTCCDate(RTCCDate* ptrToUserVal)
{
	ptrToUserVal-> Day = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DAY_REG) & 0x07);
	ptrToUserVal-> Date = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DATE_REG) & 0x7F); 
	ptrToUserVal-> Month = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MONTH_REG);
	ptrToUserVal-> Year = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_YEAR_REG) & 0x7F);
}

/********************************************************************
 * Function:        void WriteRTCCDate(RTCCDate* ptrToUserVal)
 *
 * PreCondition:    RTCCDate structure must have been declared.
 *
 * Input:           Address of typedefined RTCCDate structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update MCP7941x Date registers with the value
 *					provided in typedefined RTCCDate structure 
 *******************************************************************/
void WriteRTCCDate(RTCCDate* ptrToUserVal)
{
	unsigned char Temp = 0;
	Temp = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DAY_REG);
	Temp = Temp & 0x38;
	ptrToUserVal-> Day = ptrToUserVal-> Day | Temp;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DAY_REG,ptrToUserVal-> Day);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DATE_REG, ptrToUserVal-> Date) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MONTH_REG, ptrToUserVal-> Month) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_YEAR_REG, ptrToUserVal-> Year) ;
}

/********************************************************************
 * Function:        void ReadRTCCAlarm0(RTCCAlarm* ptrToUserVal)
 *
 * PreCondition:    RTCCAlarm structure must have been declared.
 *
 * Input:           Address of typedefined RTCCAlarm structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update typedefined RTCCAlarm structure with Alarm0
 *					read from MCP7941x 
 *******************************************************************/
void ReadRTCCAlarm0(RTCCAlarm* ptrToUserVal)
{
	ptrToUserVal-> Sec = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_SEC_REG) & 0x7F); 
	ptrToUserVal-> Min = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_MIN_REG);
	ptrToUserVal-> Hour = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_HOUR_REG) & 0x7F);
	ptrToUserVal-> DummyVal.DVal = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_DAY_REG) ;
	ptrToUserVal-> Date = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_DATE_REG);
	ptrToUserVal-> Month = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_MONTH_REG) & 0x1F);
}

/********************************************************************
 * Function:        void WriteRTCCAlarm0(RTCCAlarm* ptrToUserVal)
 *
 * PreCondition:    RTCCAlarm structure must have been declared.
 *
 * Input:           Address of typedefined RTCCAlarm structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update MCP7941x Alarm0 registers with the value
 *					provided in typedefined RTCCAlarm structure 
 *******************************************************************/
void WriteRTCCAlarm0(RTCCAlarm* ptrToUserVal)
{
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_SEC_REG, ptrToUserVal-> Sec) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_MIN_REG,ptrToUserVal-> Min);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_HOUR_REG,ptrToUserVal-> Hour);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_DAY_REG,ptrToUserVal-> DummyVal.DVal);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_DATE_REG, ptrToUserVal-> Date) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_MONTH_REG, ptrToUserVal-> Month) ;
}

/********************************************************************
 * Function:        void ReadRTCCAlarm1(RTCCAlarm* ptrToUserVal)
 *
 * PreCondition:    RTCCAlarm structure must have been declared.
 *
 * Input:           Address of typedefined RTCCAlarm structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update typedefined RTCCAlarm structure with Alarm1
 *					read from MCP7941x 
 *******************************************************************/
void ReadRTCCAlarm1(RTCCAlarm* ptrToUserVal)
{
	ptrToUserVal-> Sec = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_SEC_REG) & 0x7F); 
	ptrToUserVal-> Min = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_MIN_REG);
	ptrToUserVal-> Hour = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_HOUR_REG) & 0x7F);
	ptrToUserVal-> DummyVal.DVal = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_DAY_REG) ;
	ptrToUserVal-> Date = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_DATE_REG);
	ptrToUserVal-> Month = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_MONTH_REG) & 0x1F);
}

/********************************************************************
 * Function:        void WriteRTCCAlarm1(RTCCAlarm* ptrToUserVal)
 *
 * PreCondition:    RTCCAlarm structure must have been declared.
 *
 * Input:           Address of typedefined RTCCAlarm structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update MCP7941x Alarm1 registers with the value
 *					provided in typedefined RTCCAlarm structure 
 *******************************************************************/
void WriteRTCCAlarm1(RTCCAlarm* ptrToUserVal)
{
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_SEC_REG, ptrToUserVal-> Sec) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_MIN_REG,ptrToUserVal-> Min);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_HOUR_REG,ptrToUserVal-> Hour);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_DAY_REG,ptrToUserVal-> DummyVal.DVal);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_DATE_REG, ptrToUserVal-> Date) ;
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_MONTH_REG, ptrToUserVal-> Month) ;
}

/********************************************************************
 * Function:        bit ReadRTCCAlarm0Flag(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          1 or 0
 *
 * Side Effects:    Clears Alarm0 Flag
 *
 * Overview:        Reads Alarm0 Flag value and returns 1 or 0
 *******************************************************************/
bit ReadRTCCAlarm0Flag(void)
{
	unsigned char Temp = 0;	
	unsigned char Temp1 = 0;
	Temp = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_DAY_REG);
	Temp1 = Temp;
	Temp = Temp & 0x08;
	if(Temp == 0x08)
	{
		//clear alarm flag
		Temp1 = Temp1 & 0xF7;
		WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM0_DAY_REG,Temp1);
		return(1);
	}
	return(0);		
}

/********************************************************************
 * Function:        bit ReadRTCCAlarm1Flag(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          1 or 0
 *
 * Side Effects:    Clears Alarm1 Flag
 *
 * Overview:        Reads Alarm1 Flag value and returns 1 or 0
 *******************************************************************/
bit ReadRTCCAlarm1Flag(void)
{
	unsigned char Temp = 0;	
	unsigned char Temp1 = 0;
	Temp = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_DAY_REG);
	Temp1 = Temp;
	Temp = Temp & 0x08;
	if(Temp == 0x08)
	{
		//clear alarm flag
		Temp1 = Temp1 & 0xF7;
		WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_ALARM1_DAY_REG,Temp1);
		return(1);
	}
	return(0);		
}

/********************************************************************
 * Function:        void ReadRTCCTimeStamp_VCC_Fail(RTCCTimeStampVcc* ptrToUserVal)
 *
 * PreCondition:    RTCCTimeStampVcc structure must have been declared.
 *
 * Input:           Address of typedefined RTCCTimeStampVcc structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update typedefined RTCCTimeStampVcc structure with Date
 *					& Time when Vcc falied
 *******************************************************************/
void ReadRTCCTimeStamp_VCC_Fail(RTCCTimeStampVcc* ptrToUserVal)
{
	ptrToUserVal->Min = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_FAIL_MIN_REG);
	ptrToUserVal->Hour = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_FAIL_HOUR_REG) & 0x7F);
	ptrToUserVal->Date = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_FAIL_DATE_REG);
	//ptrToUserVal->Day_Month = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_FAIL_DAY_MONTH_REG);
	ptrToUserVal->Day_Month.DM = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_FAIL_DAY_MONTH_REG);
}

/********************************************************************
 * Function:        void ReadRTCCTimeStamp_VCC_Restore(RTCCTimeStampVcc* ptrToUserVal)
 *
 * PreCondition:    RTCCTimeStampVcc structure must have been declared.
 *
 * Input:           Address of typedefined RTCCTimeStampVcc structure
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Update typedefined RTCCTimeStampVcc structure with Date
 *					& Time when Vcc restored
 *******************************************************************/
void ReadRTCCTimeStamp_VCC_Restore(RTCCTimeStampVcc* ptrToUserVal)
{
	ptrToUserVal-> Min = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_RESTORE_MIN_REG);
	ptrToUserVal-> Hour = (READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_RESTORE_HOUR_REG) & 0x7F);
	ptrToUserVal-> Date = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_RESTORE_DATE_REG);
	ptrToUserVal-> Day_Month.DM = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_VCC_RESTORE_DAY_MONTH_REG);
}

/********************************************************************
 * Function:        void ClearRTCCTimeStamp(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Clears Vcc fail and Restore Time Stamp registers of MCP7941x
 *******************************************************************/
void ClearRTCCTimeStamp(void)
{
	unsigned char Temp = 0;
	Temp = READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DAY_REG);
	Temp = Temp & (~RTCC_EN_VBAT);
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_DAY_REG,Temp);
}

/********************************************************************
 * Function:        void RTCC_OSC_OFF(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Stops oscilator of MCP7941x. Restart RTCC call InitRTCC()
 *******************************************************************/
void RTCC_OSC_OFF(void)
{
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_SEC_REG,0x00);
}

/********************************************************************
 * Function:        char ReadRTCC_Cal_Reg(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          8bit signed value of calibration register of MCP7941x
 *
 * Side Effects:    None
 *
 * Overview:        Reads calibration register of MCP7941x
 *******************************************************************/
char ReadRTCC_Cal_Reg(void)
{
	return(READ_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_CAL_REG));
}

/********************************************************************
 * Function:        void WriteRTCC_Cal_Reg(char Cal_Val)
 *
 * PreCondition:    None
 *
 * Input:           8bit signed value
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Reads calibration register of MCP7941x
 *******************************************************************/
void WriteRTCC_Cal_Reg(char Cal_Val)
{
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,RTCC_CAL_REG,Cal_Val);
}

/********************************************************************
 * Function:        unsigned char ReadSRAMByte(unsigned char Add_Val)
 *
 * PreCondition:    None
 *
 * Input:           Address of SRAM to be read from MCP7941x
 * 
 * Output:          Returns value read from SRAM of MCP7941x
 *
 * Side Effects:    None
 *
 * Overview:        Reads a byte stored in SRAM of MCP7941x  
 *******************************************************************/
unsigned char ReadSRAMByte(unsigned char Add_Val)
{
	return(READ_I2C(CNTRL_BYTE_SRAM_RTCC,Add_Val));
}

/********************************************************************
 * Function:        void WriteSRAMByte(unsigned char Add_Val,unsigned char Data_Val)
 *
 * PreCondition:    None
 *
 * Input:           Address of SRAM and data to be written to MCP7941x
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Writes a byte to specified address of SRAM of MCP7941x  
 *******************************************************************/
void WriteSRAMByte(unsigned char Add_Val,unsigned char Data_Val)
{
	WRITE_I2C(CNTRL_BYTE_SRAM_RTCC,Add_Val,Data_Val);	
}

/********************************************************************
 * Function:        void ReadSRAMBlock(unsigned char *ptrtoVal)
 *
 * PreCondition:    None
 *
 * Input:           Address of buffer from where SRAM block read values are to be stored
 *
 * Output:          Updates the buffer with SRAM data 
 *					
 * Side Effects:    None
 *
 * Overview:        Reads a block of SRAM from MCP7941x and updates the buffer pointed by "ptrtoVal"  
 *******************************************************************/
void ReadSRAMBlock(unsigned char *ptrtoVal)
{
	ReadI2CMultipleByte(CNTRL_BYTE_SRAM_RTCC,SRAM_BLOCK_START_ADD,SRAM_BLOCK_SIZE,ptrtoVal);
}

/********************************************************************
 * Function:        void WriteSRAMBlock(unsigned char *ptrtoVal)
 *
 * PreCondition:    Buffer pointed by "ptrtoVal" should have been updated with required data
 *
 * Input:           Address of a buffer where data is to be read and update SRAM
 *
 * Output:          None
 *					
 * Side Effects:    None
 *
 * Overview:        Writes a block of SRAM from MCP7941x with the data contained in buffer pointed by "ptrtoVal"  
 *******************************************************************/
void WriteSRAMBlock(unsigned char *ptrtoVal)
{
	WriteI2CMultipleByte(CNTRL_BYTE_SRAM_RTCC,SRAM_BLOCK_START_ADD,SRAM_BLOCK_SIZE,ptrtoVal);
}

/********************************************************************
 * Function:        void WriteEEPROMByte(unsigned char Add_Val,unsigned char Data_Val)
 *
 * PreCondition:    None
 *
 * Input:           Address of EEPROM and data to be written to EEPROM of MCP7941x
 *
 * Output:         	None
 *
 * Side Effects:    None
 *
 * Overview:        Writes a byte to specified address of EEPROM of MCP7941x  
 *******************************************************************/
void WriteEEPROMByte(unsigned char Add_Val,unsigned char Data_Val)
{
	WRITE_I2C(CNTRL_BYTE_EEPROM_RTCC,Add_Val,Data_Val);	
}

/********************************************************************
 * Function:        unsigned char ReadEEPROMByte(unsigned char Add_Val)
 *
 * PreCondition:    None
 *
 * Input:           Address be read from EEPROM of MCP7941x
 *
 * Output:          Returns value read from EEPROM of MCP7941x
 *
 * Side Effects:    None
 *
 * Overview:        Reads a byte stored in EEPROM of MCP7941x  
 *******************************************************************/
unsigned char ReadEEPROMByte(unsigned char Add_Val)
{
	return(READ_I2C(CNTRL_BYTE_EEPROM_RTCC,Add_Val));
}

/********************************************************************
 * Function:        void WriteEEPROMPage(unsigned char *ptrtoVal, unsigned char Add_Val)
 *
 * PreCondition:    Buffer pointed by "ptrtoVal" should have been updated with required data
 *
 * Input:           Address of a buffer from where data is to be read and update EEPROM. And begining of Page address
 *
 * Output:          None
 *					
 * Side Effects:    The second argument of this function must be the begining of page address. Refer MCP7941x datasheet for details
 *
 * Overview:        Writes a page of EEPROM from MCP7941x with the data contained in buffer pointed by "ptrtoVal"  
 *******************************************************************/
void WriteEEPROMPage(unsigned char *ptrtoVal, unsigned char Add_Val)
{
	WriteI2CMultipleByte(CNTRL_BYTE_EEPROM_RTCC,Add_Val,EEPROM_PAGE_SIZE,ptrtoVal);
}

/********************************************************************
 * Function:        void ReadEEPROMPage(unsigned char *ptrtoVal,unsigned Add_Val)
 *
 * PreCondition:    None
 *
 * Input:           Address of a buffer from where data is to be written.And begining of the Page address
 *
 * Output:          None
 *					
 * Side Effects:    None
 *
 * Overview:        Reads a page of EEPROM from MCP7941x with the data contained in buffer pointed by "ptrtoVal"  
 *******************************************************************/
void ReadEEPROMPage(unsigned char *ptrtoVal,unsigned Add_Val)
{
	ReadI2CMultipleByte(CNTRL_BYTE_EEPROM_RTCC,Add_Val,EEPROM_PAGE_SIZE,ptrtoVal);
}

/********************************************************************
 * Function:        void ReadEEPROMArray(unsigned char *ptrtoVal,unsigned Add_Val, unsigned char Num_Data)
 *
 * PreCondition:    None
 *
 * Input:           Address of a buffer from where data is to be written . Begining of the address of EEPROM from
 *					where data is to be read. Number of bytes to be read
 *
 * Output:          None
 *					
 * Side Effects:    None
 *
 * Overview:        Reads a number of bytes(Num_Data) of EEPROM fromMCP7941x with the read data to be written
 *					to the buffer pointed by "ptrtoVal"  
 *******************************************************************/
void ReadEEPROMArray(unsigned char *ptrtoVal,unsigned Add_Val, unsigned char Num_Data)
{
	ReadI2CMultipleByte(CNTRL_BYTE_EEPROM_RTCC,Add_Val,Num_Data, ptrtoVal);
}

/********************************************************************
 * Function:        bit EEPROMBusy(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          1  or 0
 *					
 * Side Effects:    None
 *
 * Overview:        Returns 1 if EEPROM is busy or 0. This function should be called by firmware before performing Write
 *					operation to EEPROM
 *******************************************************************/
bit EEPROMBusy(void)
{
	return(CHECK_I2C_ACK(CNTRL_BYTE_EEPROM_RTCC));
}

/********************************************************************
 * Function:        void WriteUniqueID(unsigned char *ptrtoVal)
 *
 * PreCondition:    Buffer pointed by "ptrtoVal" should have been updated with unique ID data
 *
 * Input:           Address of the buffer which holds unique ID values
 *
 * Output:          None
 *					
 * Side Effects:    None
 *
 * Overview:        Writes Unique ID to MCP7941x. Necessary unlock sequences are performed by
 *					this function
 *******************************************************************/
void WriteUniqueID(unsigned char *ptrtoVal)
{
	WriteSRAMByte(0x09,0x55);
	WriteSRAMByte(0x09,0xAA);
	WriteEEPROMPage(ptrtoVal,UNIQUE_ID_START_ADD);
}


/********************************************************************
 * Function:        void ReadUniqueID(unsigned char *ptrtoVal)
 *
 * PreCondition:    None
 *
 * Input:           Address of the buffer which needs to holds unique ID values read from MCP7941x
 *
 * Output:          None
 *					
 * Side Effects:    None
 *
 * Overview:        Reads Unique ID from MCP7941x
 *******************************************************************/
void ReadUniqueID(unsigned char *ptrtoVal)
{
	ReadEEPROMPage(ptrtoVal,UNIQUE_ID_START_ADD);
}
