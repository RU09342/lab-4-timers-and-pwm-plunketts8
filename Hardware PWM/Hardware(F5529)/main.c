#include <msp430.h> 


/**
 * main.c
 * HardwarePWM
 */
volatile unsigned int count;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

	//ins and outs
    P4DIR |=BIT7;                           
    P4OUT &= ~BIT7;                        

    //pull up resistor
    P2DIR  &= ~BIT1;
    P2OUT |= BIT1;
    P2REN |= BIT1;

    // Reset/Set Mode, sets up Timer
    TA0CCTL1 = OUTMOD_7;
    TA0CTL = TASSEL_2 + MC_1 + TACLR ;
    TA0CCR0 = 100-1;
    TA0CCR1 = 50;


    while(1)
    {
        if(!(P1IN & BIT1))
        {
            P4OUT |= BIT7; //Sets P4.7


	if(TA0CCR1 <= 90) 
            {
                TA0CCR0 = 0; // Reset CCR0
                TA0CCR1 += 10; 
                TA0CCR0 = 100; //
            }


	else if (TA0CCR1 == 100){ // If the brightness is at 100%
                TA0CCR0 = 0; // Reset CCR0
                TA0CCR1 = 0; // Reset CCR1
                TA0CCR0 = 100; //  Set CCR0 back to 10 kHz
            }
        }


	if((P1IN & BIT1))
            P4OUT &= ~BIT7; //Clear P4.7
       
	    // Debounce
	for(count=100;count>0;count--)
        {
        __delay_cycles(500);
        }
    }
}
