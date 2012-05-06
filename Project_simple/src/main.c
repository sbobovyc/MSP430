#include "msp430x22x4.h"
#include "RTCC_Driver.h"

RTCCTimeDate		Rtcctimedate;			//Declare a structure

void init_io(void) {
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	P1DIR |= 0x01; // Set P1.0 to output direction
	P2IE |= 0x01; // P2.0 interrupt enabled
	P2IES |= 0x01; // P2.0 Hi/lo edge
	P2IFG &= ~0x01; // P2.0 IFG cleared
}

void init_cc(void) {
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
}

/*
 * main.c
 */
void main(void) {
	init_io();
	init_cc();
	__bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt

}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P1OUT ^= 0x01;                            // P1.0 = toggle
  P2IES ^= 0x01;                            // P2.0 toggle which edge causes interrupt
  P2IFG &= ~0x01;                           // P1.2 IFG cleared
  ReadRTCCTimeDate(&Rtcctimedate);
  __no_operation();
}
