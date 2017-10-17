
#include <msp430.h>
#include <Math.h>

/*
 * main.c
 * Software PWM
 */

int count;
int taps = 10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P1DIR |=BIT1; //set Port 9.4 output ---LED
    P1OUT &= ~BIT1; //Clear P9.4

    P5DIR  &= ~BIT6;                        // Set P1.1 as input
    P5OUT |= BIT6;                          // Configure P1.1 for Pull-Up
    P5REN |= BIT6;                          // Enable Pull Up of P1.1


    P5IE |= BIT6; //enable the interrupt on Port 1.1
    P5IES &= ~BIT6; //set as falling edge
    P5IFG &= ~(BIT6); //clear interrupt flag


    //Timer set up
    TA0CTL = TASSEL_2 + MC_1 ;
    TA0CCTL1 = (CCIE);
    TA0CCTL0 = (CCIE);
    TA0CCR0 = 1000-1;
    TA0CCR1 = 500;

    //enable global interrupt
    __bis_SR_register(GIE);


while(1)
    {
        if((P5IN & BIT6))
            P1OUT &= ~BIT1; //Clear P9.4

    }
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



#pragma vector=PORT5_VECTOR
__interrupt void PORT5_IRS(void)
{
    P5IE &= ~BIT6;
    __delay_cycles(1000);
    P5IE |= BIT6;

    P1OUT |= BIT1; //Sets P9.4

 if(TA0CCR1 < 1000)
    {
        TA0CCR0 = 0;
        TA0CCR1 += 100;
        TA0CCR0 = 1000;
    }

 else if (TA0CCR1 >= 1000){
        TA0CCR0 = 0;
        TA0CCR1 = 0;
        TA0CCR0 = 1000;
    }
    P5IFG &= ~BIT6;
}
