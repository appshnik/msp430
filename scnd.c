#include <msp430.h>

main()
{
	int i;	

	P1DIR = 0x41;
	while (1) {
		if (P1OUT != 0x01) {
			i = 0;
			P1OUT = 0x01;
			while (i < 30000)
				i++;
		}
		else {
			i = 0;
			P1OUT = 0x40;
			while (i < 30000)
				i++;
		}
	}
}
