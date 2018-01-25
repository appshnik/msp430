make : main.c
	msp430-gcc -mmcu=msp430g2553 -mdisable-watchdog main.c -o main.out
	make load
load: main.out
	mspdebug rf2500
