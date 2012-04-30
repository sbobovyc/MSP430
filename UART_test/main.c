//******************************************************************************
//   MSP430F22x4 Demo - USCI_A0, 9600 UART Echo ISR, DCO SMCLK
//
//   Description: Echo a received character, RX ISR used. Normal mode is LPM0.
//   USCI_A0 RX interrupt triggers TX Echo.
//   Baud rate divider with 1MHz = 1MHz/9600 = ~104.2
//   ACLK = n/a, MCLK = SMCLK = CALxxx_1MHZ = 1MHz
//
//                MSP430F22x4
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |     P3.4/UCA0TXD|------------>
//            |                 | 9600 - 8N1
//            |     P3.5/UCA0RXD|<------------
//
//   A. Dannenberg
//   Texas Instruments Inc.
//   April 2006
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.41A
//******************************************************************************

//******************************************************************************
// Modified by Stanislav Bobovych
// Works on CCE Version 5.1
// 1. Show LPM0, entering and exiting
// 2. Show no interrupts and what happens when stuck in power mode
// 3. Sending float
// http://www.ti.com/product/msp430f2274
// http://www.ti.com/lit/ds/symlink/msp430f2274.pdf
// http://www.ti.com/lit/ug/slau144i/slau144i.pdf
//******************************************************************************

#include  "msp430x22x4.h"

int send_byte(int);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;
  P3SEL = 0x30;                             // P3.4,5 = USCI_A0 TXD/RXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

  float value = 1.2365;
  unsigned char buffer[4];
  memcpy(buffer, &value, sizeof(float));


  while(1) {
//	  send_byte('T');
//	  send_byte('e');
//	  send_byte('s');
//	  send_byte('t');
//	  send_byte('\r');
//	  send_byte('\n');

	  send_byte(buffer[0]);
	  send_byte(buffer[1]);
	  send_byte(buffer[2]);
	  send_byte(buffer[3]);
	  //__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  }

}


int send_byte(int _c)
{
	while(!(IFG2 & UCA0TXIFG));

	UCA0TXBUF = (unsigned char) _c;

	return((unsigned char)_c);
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	__no_operation();
	while (!(IFG2 & UCA0TXIFG));                // USCI_A0 TX buffer ready?
	UCA0TXBUF = UCA0RXBUF;						// TX -> RXed character
	send_byte('\r');
	send_byte('\n');
	IFG2 &= 0x00;							  	// clear RX/TX interrupt
	__bic_SR_register_on_exit(LPM0_bits);	  	// Exit LPM0
}



