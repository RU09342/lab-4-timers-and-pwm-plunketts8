# Software PWM

The goal of this lab was to write a program that would utilize hardware techniques to synthesize PWM on an MSP430.

## Implementation

This part of the lab is very similair to the software PWM section. The difference being that this section uses the onboard PWM hardware within Timer A. 

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
The code below shows the bulk of the functionality of the hardware PWM as well as the extra work. Since brightness increases at a logarithmic scale, the logNum equation changes the brightness in increments of 10.  
```c
while(1)
    {
        if(!(P1IN & BIT1))
        {
            P9OUT |= BIT7; //Sets P9.4
            if(taps > 0)
            {
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
```
The user presses the button to increase the brightness of the LED until it reaches max level (10) and then resets.
# Original Assignment
# Hardware PWM
Now that you have done the software version of PWM, now it is time to start leveraging the other features of these Timer Modules.

## Task
You need to replicate the same behavior as in the software PWM, only using the Timer Modules ability to directly output to a GPIO Pin instead of managing them in software. 

### Hints 
Read up on the P1SEL registers as well as look at the Timer modules ability to multiplex.

## Extra Work
### Using ACLK
Some of these microprocessors have a built in ACLK which is extremely slow compared to your up to 25MHz available on some of them. What is the overall impact on the system when using this clock? Can you actually use your PWM code with a clock that slow?

### Ultra Low Power
Using a combination of ACLK, Low Power Modes, and any other means you may deem necessary, optimize this PWM code to run at 50% duty cycle with a LED on the MSP430FR5994. In particular, time how long your code can run on the fully charged super capacitor. You do not need to worry about the button control in this case, and you will probably want to disable all the GPIO that you are not using (nudge, nudge, hint, hint).