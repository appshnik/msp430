#include <msp430.h>

#define RED_LED			(BIT0)
#define GREEN_LED		(BIT6)
#define S2_BUT			(BIT3)
#define OUT_BIT			(BITF)

#define LIGHT_RED		(P1OUT |= RED_LED) : (P1OUT &= ~RED_LED)
#define LIGHT_GREEN		(P1OUT |= GREEN_LED) : (P1OUT &= ~GREEN_LED)
#define TOGGLE_RED		(P1OUT ^= RED_LED)
#define TOGGLE_GREEN		(P1OUT ^= GREEN_LED)

#define S2_PRESSED		(!(P1IN & S2_BUT))

unsigned lfsr(unsigned reg)
{
	unsigned bit;
	
	bit = ((reg >> 0)^(reg >> 2)^(reg >> 3)^(reg >> 5)) & 1;
	return ((reg >> 1) | (bit << 15));
}

unsigned glsr(unsigned reg)
{
	unsigned lsb = reg & BIT0;
	
	reg >>= 1;
	return (reg ^= (-lsb) & 0xB400);
}

int main(void)
{
	// define P1.0 and P1.6 as outputs
	P1DIR |= RED_LED | GREEN_LED & ~S2_BUT;
	// initialize TIMER_A
	TACTL |= TASSEL_2 | MC_2 | ID_1;
	
	unsigned lfsreg = 0xAAAA;
	unsigned glsreg = 0xBBBB;
	
	while(1) {
		if (TACTL & TAIFG) {
			TACTL &= ~TAIFG;
			lfsreg = lfsr(lfsreg);
			glsreg = glsr(glsreg);
		}
		// light up leds if corresponding out bit exists
		(lfsreg & OUT_BIT) ? LIGHT_GREEN;
		(glsreg & OUT_BIT) ? LIGHT_RED;
	}
	return 0;
}
