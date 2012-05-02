/*
 * main.c
 */
#include "msp430x22x4.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR |= 0x01;                            // Set P1.0 to output direction
  P2IE |= 0x01;                             // P2.0 interrupt enabled
  P2IES |= 0x01;                            // P2.0 Hi/lo edge
  P2IFG &= ~0x01;                           // P2.0 IFG cleared

  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
  P1OUT ^= 0x01;                            // P1.0 = toggle
  P2IES ^= 0x01;                            // P2.0 toggle which edge causes interrupt
  P2IFG &= ~0x01;                           // P1.2 IFG cleared
}
