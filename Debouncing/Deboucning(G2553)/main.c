#include <msp430.h> 


/**
 * main.c
 * Debouncing
 */

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

    P1SEL =0; //Select GPIO option
    P1DIR |=BIT0; //set Port 1.0 output ---LED
    P1DIR &=~(BIT3); //set Port 1.1 input --- pushbutton
    P1REN|=BIT3;//enable pull-up/pull-down resistor on
    P1OUT|=BIT3; //choose the pull-up resistor

    P1IE |=BIT3;
    P1IES |=BIT3;
    P1IFG &=~(BIT3);//clear interrupt flag

    //lpm Notice LPM0 not LPM4 like other boards
    _BIS_SR(LPM0_bits + GIE);
}




//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    int time=2000;
    
	
	P1OUT ^=0x01;
    P1IE &= ~BIT3;
    __delay_cycles(1);

    //Debounce
    TA0CTL = TASSEL_1 + MC_1 + ID_1; //Set up Timer A, Count up, divider 2
    TA0CCTL0 = 0x10; // Set up compare mode for CCTL
    TA0CCR0 = time;

    P1IFG &=~(BIT3); // Clear flag
}



#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P1IE |= BIT3; //Enable interrupt again.

}
