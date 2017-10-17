
/**
 * main.c
 * Software PWM
 */
#include <msp430.h>
#include <Math.h>

volatile unsigned int j;
int taps = 10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P9DIR |=BIT7; 							//set Port 9.4 output 
    P9OUT &= ~BIT7; 						//Clear P9.4
    P1DIR  &= ~BIT1;                        // Set P1.1 as input
    P1OUT |= BIT1;                          // Configure P1.1 for Pull-Up
    P1REN |= BIT1;                          // Enable Pull Up of P1.1
    
	P1IE |= BIT1; 
    P1IES &= ~BIT1; 
    P1IFG &= ~(BIT1); 

     // SMCLK / Upmode
    TA0CTL = TASSEL_2 + MC_1 ;      
    TA0CCTL1 = (CCIE);
    TA0CCTL0 = (CCIE);
    //Ask Dr.Tang about this line from class
    TA0CCR0 = 1000-1;                       
    TA0CCR1 = 500;                           

    __bis_SR_register(GIE);



while(1)
    {
        if((P1IN & BIT1))
            P9OUT &= ~BIT7; //Clear P9.4

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



#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT1;
    __delay_cycles(100);
    P1IE |= BIT1;

    P9OUT |= BIT7; //Sets P9.4
    if(TA0CCR1 < 1000)
    {
        int x = TA0CCR1 + 100;
        TA0CCR0 = 0;
        TA0CCR1 = x;
        TA0CCR0 = 100;
    }
    else if (TA0CCR1 >= 1000){
        TA0CCR0 = 0;
        TA0CCR1 = 0;
        TA0CCR0 = 1000;
    }
    P1IFG &= ~BIT1;
}

