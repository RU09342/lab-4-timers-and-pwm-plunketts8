#include <msp430.h> 


/**
 * main.c
 */
volatile unsigned int count;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P1DIR |=BIT6; //set Port 1.6 output ---LED
    P1OUT &= ~BIT6; //Clear P1.6

    P1DIR  &= ~BIT3;                        // Set P1.3 as input
    P1OUT |= BIT3;                          // Configure P1.3 for Pull-Up
    P1REN |= BIT3;                          // Enable Pull Up of P1.3
    P1IE |= BIT3; //enable the interrupt on Port 1.3
    P1IES &= ~BIT3; //set as falling edge
    P1IFG &= ~(BIT3); //clear interrupt flag


    TA0CTL = TASSEL_2 + MC_1 ;
    TA0CCTL1 = (CCIE);
    TA0CCTL0 = (CCIE);
    TA0CCR0 = 1000-1;
    TA0CCR1 = 500;

    __bis_SR_register(GIE);

while(1)
    {
        if((P1IN & BIT3))
            P1OUT &= ~BIT6; //Clear P1.6

    }
}




#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT3;

    //Debounce
    __delay_cycles(1000);
    P1IE |= BIT3; //Port 1 interrupt enable is turned back on

    P1OUT |= BIT6; //Sets P1.6

    if(TA0CCR1 >= 1000) // If the brightness is at 100%
    {
        TA0CCR0 = 0; // Reset CCR0
        TA0CCR1 = 0;// Reset CCR1
        TA0CCR0 = 1000; //  Set CCR0 back to 10 kHz
    }

    else if (TA0CCR1 < 1000){
        TA0CCR0 = 0;
        TA0CCR1 += 100;
        TA0CCR0 = 1000;//  Set CCR0 back to 10 kHz


    }
    P1IFG &= ~BIT3; //Clear flag
}





//Timer A interrupt vectors
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void)
{
    if(TA0CCR1 != 1000)
    {
       P1OUT &= ~(BIT0); //turns off red led
    }
    TA0CCTL1 &= ~BIT0; //clears flag
}




#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(TA0CCR1 != 0){
        P1OUT |= (BIT0); //turns on red led
    }
    TA0CCTL0 &= ~BIT0;  //clears flag
}
