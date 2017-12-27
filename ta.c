#include <msp430.h>

#define RED_LED			(BIT0)
#define GREEN_LED		(BIT6)
#define TOGGLE_RED		(P1OUT ^= RED_LED)
#define TOGGLE_GREEN		(P1OUT ^= GREEN_LED)

int main(void)
{
	// define P1.0 and P1.6 as outputs
	P1DIR |= RED_LED | GREEN_LED;
	// light up GREEN_LED
	P1OUT |= GREEN_LED & ~RED_LED;
	// initialize TIMER_A0
	TACTL |= TACLR;
	TACTL |= TASSEL_2 | MC_2 | ID_3;
	
	while(1) {
		if (TACTL & TAIFG) {
			TACTL &= ~TAIFG;
			TOGGLE_RED;
			TOGGLE_GREEN;
		}
	}

	return 0;
}
