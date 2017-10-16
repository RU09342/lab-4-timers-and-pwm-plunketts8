# Software PWM

The goal of this lab was to write a program that would utilize software techniques to synthesize PWM on an MSP430.

## Implementation

Similair to the other sections of this lab the correct pins need to be initialized as inputs and outputs. 

```c
	WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD
```
The pull up or pull down resistor should be enabled and then selected. In the example below the pull up is chosen.

```c
	P1REN|=BIT1;
	P1OUT|=BIT1; 
```

The below code shows how to set up Timer A, SMCLK in up mode.
```c
	TA0CTL = TASSEL_2 + MC_1 ;

```

Using the method discussed in class, TA0CCR0 is initialized at 1000-1=999. TA0CCR1 is initialized at 500 setting the duty cycle to 50% at 1 kHz. The code below shows how to implement this.
```c
	TA0CCTL1 = (CCIE);
	TA0CCTL0 = (CCIE);
	TA0CCR0 = 1000-1;                        
	TA0CCR1 = 500;                           

```

Set Low Power Mode:
```c
	 _BIS_SR(LPM4_bits + GIE);
```

Below shows the Port 1 interrupt service routine. This function is called when the button is pressed.
```c
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT1;
    __delay_cycles(100);
    P1IE |= BIT1;

    P9OUT |= BIT7; //Sets P9.4
    if(TA0CCR1 < 1000)
    {
        int incrementNum = TA0CCR1 + 100;
        TA0CCR0 = 0;
        TA0CCR1 = incrementNum;
        TA0CCR0 = 100;
    }
    else if (TA0CCR1 >= 1000){
        TA0CCR0 = 0;
        TA0CCR1 = 0;
        TA0CCR0 = 1000;
    }
    P1IFG &= ~BIT1;
}
```
Collaborated with Matt Rodriquez and Austin Huang.

# Original Assignment
# Software PWM
Most microprocessors will have a Timer module, but depending on the device, some may not come with pre-built PWM modules. Instead, you may have to utilize software techniques to synthesize PWM on your own.

## Task
You need to generate a 1kHz PWM signal with a duty cycle between 0% and 100%. Upon the processor starting up, you should PWM one of the on-board LEDs at a 50% duty cycle. Upon pressing one of the on-board buttons, the duty cycle of the LED should increase by 10%. Once you have reached 100%, your duty cycle should go back to 0% on the next button press. You also need to implement the other LED to light up when the Duty Cycle button is depressed and turns back off when it is let go. This is to help you figure out if the button has triggered multiple interrupts.

### Hints
You really, really, really, really need to hook up the output of your LED pin to an oscilloscope to make sure that the duty cycle is accurate. Also, since you are going to be doing a lot of initialization, it would be helpful for all persons involved if you created your main function like:
'''c
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	LEDSetup(); // Initialize our LEDS
	ButtonSetup();  // Initialize our button
	TimerA0Setup(); // Initialize Timer0
	TimerA1Setup(); // Initialize Timer1
	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}
'''
This way, each of the steps in initialization can be isolated for easier understanding and debugging.

Collaborated with Matt Rodriquez and Austin Huang.


## Extra Work
### Linear Brightness
Much like every other things with humans, not everything we interact with we perceive as linear. For senses such as sight or hearing, certain features such as volume or brightness have a logarithmic relationship with our senses. Instead of just incrementing by 10%, try making the brightness appear to change linearly. 

### Power Comparison
Since you are effectively turning the LED off for some period of time, it should follow that the amount of power you are using over time should be less. Using Energy Trace, compare the power consumption of the different duty cycles. What happens if you use the pre-divider in the timer module for the PWM (does it consume less power)?