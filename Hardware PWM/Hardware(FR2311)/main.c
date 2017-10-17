#include <msp430.h> 


/**
 * main.c
 * Hardware PWM
 */

volatile unsigned int count;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off

	//ins and outs
    P2DIR |=BIT0;                           
    P2OUT &= ~BIT0;                         

	//pull up resistor
    P1DIR  &= ~BIT1;                       
    P1OUT |= BIT1;                          
    P1REN |= BIT1;                          

	//SMCLK and pwm frequency
    TB0CCTL1 = OUTMOD_7;                    
    TB0CTL = TBSSEL_2 + MC_1 +TBCLR ;       
    TB0CCR0 = 100-1;                       
    TB0CCR1 = 50;                         


while(1)
    {

    if(!(P1IN & BIT1)) //interrupt
        {
            P2OUT |= BIT0;


    if(TB0CCR1 <= 90) 
            {
                TB0CCR0 = 0; // Reset CCR0
                TB0CCR1 += 10; // Add 10%
                TB0CCR0 = 100; // Set CCR0 back to 10 kHz
            }
    else if (TB0CCR1 == 100){ // If the brightness is at 100%
                TB0CCR0 = 0; // Reset CCR0
                TB0CCR1 = 0; // Reset CCR1
                TB0CCR0 = 100; //  Set CCR0 back to 10 kHz
            }
        }


    if((P1IN & BIT1))
            P2OUT &= ~BIT0; 
        
		
		// Debounce
    for(count=100;count>0;count--)
        {
        __delay_cycles(500);
        }
    }
}
