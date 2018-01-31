	org 0xf800
start:
	// stop the watchdog
	mov.w #0x5A80, &0x0120;
	// configure P1DIR
	mov.b #0x1, &0x22
	// configure MCLK
	mov.b #0x30, &0x58
main:
	mov.w #0x0, R4	
	// toggle P1.0 out pin
	xor.b #0x01, &0x21
incr:	
	inc.w R4
	jnc incr
	jmp main
	org 0xfffe
	dw start
