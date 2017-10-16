# Debouncing

The goal of this lab was to write a program that would implement a debounced switch to control the state of an LED.
## Implementation

Any microcontroller that uses buttons or other mechnical inputs must account for bouncing. One attempt user input should translate to one output. This program reduces the effects of bouncing by enabling the pull up resistor on the input button and initializing interrupts on it's rising or falling edge. 

Simply put the processor should wait a set amount of time before it starts "accepting" another input. 


The following code sets up port 1.1 as an input, enables the pull up/ pull down resistor, then chooses pull up
```c
	P1DIR &=~(BIT1); 
	P1REN|=BIT1;
	P1OUT|=BIT1; 

```

Enter low power mode and enable global interrupt
```c
	_BIS_SR(LPM4_bits + GIE); 

```
The following code acts as a debounce. It sets up Timer A, ,sets CCTL to compare mode, and sets the duration at which the interrupt is disabled. 
```c
TA0CTL = TASSEL_1 + MC_1 + ID_1; 
    TA0CCTL0 = 0x10; 
    TA0CCR0 = 3000;
```

The end of the program should have a line that renables the interrupt
```c
	P1IE |= BIT1;
```

Collaborated with Matt Rodriquez and Austin Huang


# Original Assignment
# Software Debouncing
In previously labs, we talked about how objects such as switches can cause some nasty effects since they are actually a mechanical system at heart. We talked about the simple hardware method of debouncing, but due to the many different design constraints, you may not be able to add or adjust hardware. Debouncing is also only one of many applications which would require the use of built in Timers to allow for other processes to take place.

## Task
You need to utilize the TIMER modules within the MSP430 processors to implement a debounced switch to control the state of an LED. You most likely will want to hook up your buttons on the development boards to an oscilloscope to see how much time it takes for the buttons to settle. The idea here is that your processor should be able to run other code, while relying on timers and interrupts to manage the debouncing in the background. You should not be using polling techniques for this assignment. Your code should also be able to detect 

### Hints
You need to take a look at how the P1IE and P1IES registers work and how to control them within an interrupt routine. Remember that the debouncing is not going to be the main process you are going to run by the end of the lab.

## Extra Work
### Low Power Modes
Go into the datasheets or look online for information about the low power modes of your processors and using Energy Trace, see what the lowest power consumption you can achieve while still running your debouncing code. Take a note when your processor is not driving the LED (or unplug the header connecting the LED and check) but running the interrupt routine for your debouncing.

### Double the fun
Can you expand your code to debounce two switches? Do you have to use two Timer peripherals to do this?
