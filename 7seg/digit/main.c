#include <msp430.h>

#define A			(BIT1)
#define B			(BIT2)
#define C			(BIT3)
#define D			(BIT4)
#define E			(BIT5)
#define F 			(BIT6)
#define G 			(BIT7)
#define DP			(BIT0) 

#define MAX_INT			(490)	// approximately 1.6 V DC
#define STEP			(49)

int conv_st = 0;	// current state of ADC
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

void out_digit(int dig)
{
	switch (dig) {
		case 0 :
			P1OUT = A|B|C|D|E|F;
			break; 
		case 1 :
			P1OUT = B|C;
			break; 
		case 2 :
			P1OUT = A|B|D|E|G;
			break; 
		case 3 :
			P1OUT = A|B|C|D|G;
			break; 
		case 4 :
			P1OUT = B|C|G|F;
			break; 
		case 5 :
			P1OUT = A|C|D|G|F;
			break; 
		case 6 :
			P1OUT = A|G|C|D|E|F;
			break; 
		case 7 :
			P1OUT = A|B|C;
			break; 
		case 8 :
			P1OUT = A|B|C|D|E|F|G;
			break; 
		case 9 :
			P1OUT = A|B|C|D|F|G;
			break;
		default:
			P1OUT = G; 
	}
}

void main(void)
{
	int i = 0;
	adc_init();

	// define port 1.1 to 1.7 as outputs
	P1DIR |= A | B | C | D | E | F | G;

	while(1) {
		conv_st = conv_going();
		
		if (!conv_st) 
			start_conv();
		if (!conv_st) {
			out_digit(ADC10MEM / STEP);
		}
		// waiting for non bouncing purpose
		while (i < 10000) i++;
		i = 0;		
	}
}
