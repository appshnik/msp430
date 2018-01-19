#include <msp430.h>

#define RED_LED			(BIT0)
#define GREEN_LED		(BIT6)
#define S2_BUT			(BIT3)

#define LIGHT_RED		(P1OUT |= RED_LED) : (P1OUT &= ~RED_LED)
#define LIGHT_GREEN		(P1OUT |= GREEN_LED) : (P1OUT &= ~GREEN_LED)
#define TOGGLE_RED		(P1OUT ^= RED_LED)
#define TOGGLE_GREEN		(P1OUT ^= GREEN_LED)

#define S2_PRESSED		(!(P1IN & S2_BUT))

__attribute__((interrupt(TIMER0_A1_VECTOR)))
timer_isr(void)
{
	TOGGLE_RED;
	TACTL &= ~TAIFG;
}

__attribute__((interrupt(PORT1_VECTOR)))
port1_isr(void)
{
	TOGGLE_GREEN;
	P1IFG &= ~P1IFG;
}
void main(void)
{
	// define P1.0 and P1.6 as outputs
	P1DIR |= RED_LED | GREEN_LED;
	// set interrupt mode for P1.3
	P1IES |= S2_BUT;	// set high-down edge detection
	P1IE |= S2_BUT;		// enable P1.3 input interrupt

	// initialize TIMER_A
	TACTL |= TASSEL_2 | MC_2 | ID_3 | TAIE;
	__enable_interrupt();
		

	while(1)
/*		if (TACTL & TAIFG) {
			TACTL &= ~TAIFG;
			TOGGLE_GREEN;
		}*/;
}
