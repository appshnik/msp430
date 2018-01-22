#include <msp430.h>

#define RED_LED			(BIT0)
#define GREEN_LED		(BIT6)
#define S2_BUT			(BIT3)
#define IN_V			(40)

#define LIGHT_RED		(P1OUT |= RED_LED) : (P1OUT &= ~RED_LED)
#define LIGHT_GREEN		(P1OUT |= GREEN_LED) : (P1OUT &= ~GREEN_LED)
#define TOGGLE_RED		(P1OUT ^= RED_LED)
#define TOGGLE_GREEN		(P1OUT ^= GREEN_LED)

#define S2_PRESSED		(!(P1IN & S2_BUT))


// initialize TIMER_A
void ta_init(void)
{
	TACTL |= TASSEL_2 | MC_2 | ID_3 | TAIE;
}

// initialize ADC10
void adc_init(void)
{	
	// Bit ADC10ON must be set before any configuration activity
	ADC10CTL0 |= ADC10ON;
	ADC10CTL0 |= SREF_0 | ADC10SHT_2;
	ADC10CTL1 |= INCH_0 | SHS_0 | ADC10SSEL_0 | ADC10DIV_0 | CONSEQ_0;
	ADC10AE0 |= BIT0;
	// Bit ENC must be set strictly after all configuration activities 
	ADC10CTL0 |= ENC;
}

// start conversion function
void start_conv(void)
{
	ADC10CTL0 |= ADC10SC;
}

// check if conversion is done
unsigned int conv_going(void)
{
	// function returns 1 if conversion routine is is done
	return (ADC10CTL1 & ADC10BUSY);
}

__attribute__((interrupt(TIMER0_A1_VECTOR)))
timer_isr(void)
{
	TACTL &= ~TAIFG;
}

void main(void)
{
	int conv_st;
	adc_init();
	P1DIR |= RED_LED;
	P1OUT = 0x0;
	while(1) {
		ADC10AE0 |= BIT0; 
		conv_st = conv_going();
		
		if (!conv_st)
			start_conv();
		if (!conv_st) {
			if (ADC10MEM > IN_V)
				ADC10AE0 &= ~BIT0; 
			(ADC10MEM > IN_V) ? LIGHT_RED;
			for (conv_st = 0; conv_st < 10000; conv_st++);
		}
	}
}


