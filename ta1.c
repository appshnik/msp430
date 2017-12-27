#include <msp430.h>

#define RED_LED			(BIT0)
#define GREEN_LED		(BIT6)
#define S2_BUT			(BIT3)
#define TOGGLE_RED		(P1OUT ^= RED_LED)
#define TOGGLE_GREEN		(P1OUT ^= GREEN_LED)
#define S2_PRESSED		(!(P1IN & S2_BUT))

int main(void)
{
	// define P1.0 and P1.6 as outputs
	P1DIR |= RED_LED | GREEN_LED & ~S2_BUT;
	// light up GREEN_LED
	P1OUT |= GREEN_LED & RED_LED;
	// initialize TIMER_A
//	TACTL |= TACLR;
	TACTL |= TASSEL_2 | MC_3 | ID_3;
	TACCR0 = 0xffff;
	// set interrupt levels
	TACCR1 = TACCR0 / 2;
	TACCTL1 |= OUTMOD_3 | CCIE;
	
	while(1) {
/*		if (S2_PRESSED)
			TACCR0 = 0x0;
		else
			TACCR0 = 0xffff;
*/
		if (TACTL & TAIFG) {
			TACTL &= ~TAIFG;
			TOGGLE_RED;
		}
		if (TACCTL1 & CCIFG) {
			TACCTL1 &= ~CCIFG;
			if (!S2_PRESSED)
				TOGGLE_GREEN;
		}
	}

	return 0;
}
