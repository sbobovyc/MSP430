/******************************************************************************************************************
File Name    			: I2C_Driver.c 
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
#include <string.h>
#include "msp430x22x4.h"
#include "I2C_Driver.h"

int RXByteCtr, RPT_Flag = 0;                // enables repeated start when 1
unsigned char TXData;
unsigned char * PRxData;
unsigned char * PTxData;
unsigned char TxBuffer[128];
unsigned char RxBuffer;
unsigned char TXByteCtr, RX = 0;

void Setup_TX(unsigned char Add);
void Setup_RX(unsigned char Add);


/********************************************************************
 * Function:        void InitI2C(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *					
 * Side Effects:    None
 *
 * Overview:        Initializes MSSP module in I2C single master mode. Default I2C clock is set to 100Khz
 *******************************************************************/
void InitI2C(void)
{
	  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	  P1DIR |= (1 << 1);
	  P1OUT |= (1 << 1);
	  P2DIR |= (1 << 1);
	  P2SEL |= (1 << 1);                        // SMCLK Output
	  P3DIR = 0x0F;								// disable CC2500 //TODO remember to reenable the radio
	  P3SEL |= 0x06;                            // Assign I2C pins to USCI_B0
	  memset(TxBuffer, 0, 128*sizeof(unsigned char));
}


/********************************************************************
 * Function:        void WRITE_I2C(unsigned char Slave_Add,unsigned char Add, unsigned char Val)
 *
 * PreCondition:    None
 *
 * Input:           I2C slave address, Address where data is to be written, Data to be written
 *
 * Output:          None
 *					
 * Side Effects:    None
 *
 * Overview:        Writes a byte to I2C slave at specified address
 *******************************************************************/
void WRITE_I2C(unsigned char Slave_Add,unsigned char Add, unsigned char Val)
{
	Setup_TX(Slave_Add);
	RPT_Flag = 0;
	TxBuffer[0] = Add;
	TxBuffer[1] = Val;
	PTxData = TxBuffer; // TX array start address
	TXByteCtr = 2; // Load TX byte counter
	while (UCB0CTL1 & UCTXSTP); // Ensure stop condition got sent
	UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);
	// Enter LPM0 w/ interrupts
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
}

/********************************************************************
 * Function:        unsigned char READ_I2C(unsigned char Slave_Add,unsigned char Add)
 *
 * PreCondition:    None
 *
 * Input:           I2C slave address, Address from where data is to be read
 *
 * Output:          Returns a byte as data read from I2C slave
 *
 * Side Effects:    None
 *
 * Overview:        Reads a byte from I2C slave from specified address
 *******************************************************************/
unsigned char READ_I2C(unsigned char Slave_Add,unsigned char Add)
{
	Setup_TX(Slave_Add);
	RPT_Flag = 1;
	PTxData = &Add; // TX array start address
	TXByteCtr = 1; // Load TX byte counter
	while (UCB0CTL1 & UCTXSTP); // Ensure stop condition got sent
	UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);
	// Enter LPM0 w/ interrupts
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

	RPT_Flag = 0;
	Setup_RX(Slave_Add);
	PRxData = &RxBuffer; // Start of RX buffer
	RXByteCtr = 1; // Load RX byte counter
	while (UCB0CTL1 & UCTXSTP); // Ensure stop condition got sent
	UCB0CTL1 |= UCTXSTT; // I2C start condition
	while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
	UCB0CTL1 |= UCTXSTP;                // No Repeated Start: stop condition
	__bis_SR_register(CPUOFF + GIE);
	// Enter LPM0 w/ interrupts
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

	return RxBuffer;
}

///********************************************************************
// * Function:        void WriteI2CMultipleByte(unsigned char Slave_Add,unsigned char Add,unsigned char Cnt_Byte, unsigned char *ptrToData)
// *
// * PreCondition:    None
// *
// * Input:           I2C slave address, Address from where data is to be written, Number of bytes to be written,
// *					Address of the buffer from where data is to be read and written to I2C slave device
// *
// * Output:          None
// *
// * Side Effects:    None
// *
// * Overview:        Writes more than a byte to I2C slave device
// *******************************************************************/
//void WriteI2CMultipleByte(unsigned char Slave_Add,unsigned char Add,unsigned char Cnt_Byte, unsigned char *ptrToData)
//{
//	CHECK_I2C_IDLE();
//
//	SEN = 1;									//Initiate Start condition on I2C bus
//	while(SEN);									//Wait for Start condition to complete
//	SSPBUF = Slave_Add;
//	CHECK_I2C_IDLE();
//	while(ACKSTAT);      						//Check for ACK from I2C slave device
//
//	SSPBUF = Add;
//	CHECK_I2C_IDLE();
//	while(ACKSTAT);      						//Check for ACK from I2C slave device
//	do
//	{
//		SSPBUF = *(ptrToData++);
//		CHECK_I2C_IDLE();
//		while(ACKSTAT);      					//Check for ACK from I2C slave device
//		Cnt_Byte--;
//	}while(Cnt_Byte != 0);
//	PEN = 1;									//Issue stop condition
//}
//
///********************************************************************
// * Function:        void ReadI2CMultipleByte(unsigned char Slave_Add,unsigned char Add,unsigned char Cnt_Byte, unsigned char *ptrToData)
// *
// * PreCondition:    None
// *
// * Input:           I2C slave address, Address from where data is to be read, Number of bytes to be read,
// *					Address of the buffer from where the read data(from I2C slave device) is to be written
// *
// * Output:          None
// *
// * Side Effects:    None
// *
// * Overview:        Reads more than a byte from I2C slave device
// *******************************************************************/
void ReadI2CMultipleByte(unsigned char Slave_Add,unsigned char Add,unsigned char Cnt_Byte, unsigned char *ptrToData)
{
	Setup_TX(Slave_Add);
	RPT_Flag = 1;
	PTxData = &Add; // TX array start address
	TXByteCtr = 1; // Load TX byte counter
	while (UCB0CTL1 & UCTXSTP); // Ensure stop condition got sent
	UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition
	__bis_SR_register(CPUOFF + GIE);
	// Enter LPM0 w/ interrupts
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

	Setup_RX(Slave_Add);
	RPT_Flag = 1;
	PRxData = ptrToData; // Start of RX buffer
	RXByteCtr = Cnt_Byte - 1;
	while (UCB0CTL1 & UCTXSTP); // Ensure stop condition got sent
	UCB0CTL1 |= UCTXSTT; // I2C start condition
	while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
	__bis_SR_register(CPUOFF + GIE);
	// Enter LPM0 w/ interrupts
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

}



//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any 2+ number of bytes by pre-loading RXByteCtr with the byte count.
//-------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  if(RX == 1){                              // Master Recieve?
	  if(RPT_Flag == 0) {
		  *PRxData = UCB0RXBUF;
		  __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
	  } else {
		  	 RXByteCtr--;                              // Decrement RX byte counter
		    if (RXByteCtr) {
		    	*PRxData++ = UCB0RXBUF;                 // Move RX data to address PRxData
		    }
		    else
		    {
		  		UCB0CTL1 |= UCTXSTP;                // No Repeated Start: stop condition
		  		*PRxData = UCB0RXBUF;                   // Move final RX data to PRxData
		  		__bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
	  }

  }

  } else{                                     // Master Transmit
      if (TXByteCtr)                        // Check TX byte counter
  {
    UCB0TXBUF = *PTxData++;                   // Load TX buffer
    TXByteCtr--;                            // Decrement TX byte counter
  }
  else
  {
    if(RPT_Flag == 1){
    RPT_Flag = 0;
    PTxData = TxBuffer;                      // TX array start address
    __bic_SR_register_on_exit(CPUOFF);
    }
    else{
    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
    }
  }
 }

}

void Setup_TX(unsigned char Add) {
	_DINT();
	RX = 0;
	IE2 &= ~UCB0RXIE;			// Disable RX interrupt
	while (UCB0CTL1 & UCTXSTP); // Ensure stop condition got sent
	UCB0CTL1 |= UCSWRST; 		// Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST; 		// Use SMCLK, keep SW reset
	UCB0BR0 = 12; // fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;
	UCB0I2CSA = Add; // Slave Address
	UCB0CTL1 &= ~UCSWRST; // Clear SW reset, resume operation
	IE2 |= UCB0TXIE; // Enable TX interrupt
}

void Setup_RX(unsigned char Add) {
	_DINT();
	RX = 1;
	IE2 &= ~UCB0TXIE;	// Disable TX interrupt
	UCB0CTL1 |= UCSWRST; // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST; // Use SMCLK, keep SW reset
	UCB0BR0 = 12; // fSCL = SMCLK/12 = ~100kHz
	UCB0BR1 = 0;
	UCB0I2CSA = Add; // Slave Address
	UCB0CTL1 &= ~UCSWRST; // Clear SW reset, resume operation
	IE2 |= UCB0RXIE; // Enable RX interrupt
}


