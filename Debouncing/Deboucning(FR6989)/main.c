#include <msp430.h> 


/**
 * main.c
 * Debouncing
 */

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |=BIT0; //set Port 1.0 output ---LED
    P1DIR &=~(BIT1); //set Port 1.1 input --- pushbutton
    P1REN|=BIT1;//enable pull-up/pull-down resistor on
    P1OUT|=BIT1; //choose the pull-up resistor

    P1IE |=BIT1;//enable the interrupt on Port 1.1
    P1IES |=BIT1;//set as falling edge
    P1IFG &=~(BIT1);//clear interrupt flag

    //lpm
    _BIS_SR(LPM4_bits + GIE);
}



//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    int time=2000;
    
	
	P1OUT ^=0x01;
    P1IE &= ~BIT1;
    __delay_cycles(1);


    //Debounce
    TA0CTL = TASSEL_1 + MC_1 + ID_1; //Set up Timer A, Count up, divider 2
    TA0CCTL0 = 0x10; // Set up compare mode for CCTL
    TA0CCR0 = time;


    P1IFG &=~(BIT1); // Clear flag
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P1IE |= BIT1; //Enable interrupt again.

}
