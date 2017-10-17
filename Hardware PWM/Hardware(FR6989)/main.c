

/**
 * main.c
 * Hardware PWM
 */

#include <msp430.h>
#include <Math.h>

volatile unsigned int count;
int taps = 10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;


    P9DIR |=BIT7; 
    P9OUT &= ~BIT7; 

	//pull up resistor
    P1DIR  &= ~BIT1;                        
    P1OUT |= BIT1;                          
    P1REN |= BIT1;                         

    //reset/set mode and initializes timer
    TA0CCTL1 = OUTMOD_7;
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;
    TA0CCR0 = 100-1;

    //sets duty cycle
    TA0CCR1 = 50;
    P1SEL0 |= BIT0;
    P1SEL1 &= ~BIT0;



    while(1)
    {
        if(!(P1IN & BIT1))
        {
            P9OUT |= BIT7; //Sets P9.7
           
		    if(taps > 0)
            {
                //extra work to increase brightness at log scale
                taps--;
                double logNum  = log10(taps) * 100.0;
                int incrementNum = (100 - logNum);
                TA0CCR0 = 0;
                TA0CCR1 = incrementNum;
                TA0CCR0 = 100;
            }


            else if (taps == 0){
                TA0CCR0 = 0;
                TA0CCR1 = 0;
                TA0CCR0 = 100;
                taps = 10;
            }
        }


        if((P1IN & BIT1))
            P9OUT &= ~BIT7; //Clear P9.7


//debounce
        for(count=100;count>0;count--)
        {
        __delay_cycles(1000);
        }
    }
}
