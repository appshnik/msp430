#include <msp430.h>

#define A			(BIT1)
#define B			(BIT2)
#define C			(BIT3)
#define D			(BIT4)
#define E			(BIT5)
#define F 			(BIT6)
#define G 			(BIT7)
#define DP			(BIT5) 

#define MAX_INT			(490)	// approximately 1.6 V DC

int conv_st = 0;	// current state of ADC
int otaccr0;		// TACCR0 value from previous cycle
// initialize TIMER_A
void ta_init(void)
{
	TACTL |= TASSEL_2 | MC_1 | ID_3 | TAIE;
	TACCR0 = MAX_TACCR0;
}

// initialize ADC10
void adc_init(void)
{	
	// Bit ADC10ON must be set before any configuration activity
	ADC10CTL0 |= ADC10ON;
	ADC10CTL0 |= SREF_0 | ADC10SHT_0;
	ADC10CTL1 |= INCH_0 | SHS_0 | ADC10SSEL_3 | ADC10DIV_0 | CONSEQ_0;
	ADC10AE0 |= BIT0;
	// Bit ENC must be set strictly after all configuration activities 
	ADC10CTL0 |= ENC;
}

// start conversion function
void start_conv(void)
{
	ADC10CTL0 |= ADC10SC;
}

// check if conversion is being executed
unsigned int conv_going(void)
{
	// function returns 1 if conversion routine is being executed
	return (ADC10CTL1 & ADC10BUSY);
}

// interrupt service routine for timer A
__attribute__((interrupt(TIMER0_A1_VECTOR)))
timer_isr(void)
{
	(P1OUT >= F) ? (P1OUT = A) : (P1OUT <<= 1);
	TACTL &= ~TAIFG;
}

void main(void)
{
	int i = 0;
	adc_init();
	ta_init();
	__enable_interrupt();

	// define port 1.1 to 1.5 as outputs
	P1DIR |= A | B | C | D | E | F;
	// light up first led
	P1OUT = A;
	// define port 2.0 as output to indicate freqency change
	P2DIR |= BIT0;

	while(1) {
		conv_st = conv_going();
		
		if (!conv_st) 
			start_conv();
		if (!conv_st) {
			TACCR0 =(unsigned int)((unsigned int)MAX_TACCR0 / (unsigned int)MAX_INT) * (unsigned int)ADC10MEM;
		}
		// waiting for non bouncing purpose
		while (i < 10000) i++;
		i = 0;		
		// check whether frequency has been changed..
		if (otaccr0 != TACCR0)
			// ..if so - toggle indicator state
			P2OUT ^= BIT0;
		otaccr0 = TACCR0;

	}
}
