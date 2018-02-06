	org 0xC000
start:
	// stop watchdog
	mov.w #0x5A80, &0x0120
	// initialize stack pointer
	mov.w #0x0400, SP
	// configure TACTL
	mov.w #0x02E2, &0x0160
	// configure PORT 1
	mov.b #0x01, &0x0022
	mov.b #0x00, &0x0021
	eint
main:
	jmp main
ta_isr:
	xor #0x01, &0x0021
	and #0xfffe, &0x0160
	reti

	org 0xFFF0
	dw ta_isr
	org 0xFFFE
	dw start
