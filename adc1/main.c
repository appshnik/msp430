#include <msp430.h>

#define LED_1			(BIT1)
#define LED_2			(BIT2)
#define LED_3			(BIT3)
#define LED_4			(BIT4)
#define LED_5			(BIT5)

#define MAX_INT			(477)
#define LVL1			(MAX_INT/6 * 1)
#define LVL2			(MAX_INT/6 * 2)
#define LVL3			(MAX_INT/6 * 3)
#define LVL4			(MAX_INT/6 * 4)
#define LVL5			(MAX_INT/6 * 5)

#define LIGHT_1			(P1OUT |= LED_1) : (P1OUT &= ~LED_1)
#define LIGHT_2			(P1OUT |= LED_2) : (P1OUT &= ~LED_2)
#define LIGHT_3			(P1OUT |= LED_3) : (P1OUT &= ~LED_3)
#define LIGHT_4			(P1OUT |= LED_4) : (P1OUT &= ~LED_4)
#define LIGHT_5			(P1OUT |= LED_5) : (P1OUT &= ~LED_5)

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
	P1DIR |= LED_1 | LED_2 | LED_3 | LED_4 | LED_5;
	P1OUT = 0x0;
	while(1) {
		conv_st = conv_going();
		
		if (!conv_st)
			start_conv();
		if (!conv_st) {
			(ADC10MEM > LVL1) ? LIGHT_1;
			(ADC10MEM > LVL2) ? LIGHT_2;
			(ADC10MEM > LVL3) ? LIGHT_3;
			(ADC10MEM > LVL4) ? LIGHT_4;
			(ADC10MEM > LVL5) ? LIGHT_5;


			//for (conv_st = 0; conv_st < 10000; conv_st++);
		}
	}
}


