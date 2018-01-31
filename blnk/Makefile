make : main.s
	naken430asm main.s
	make load
load: out.hex
	mspdebug rf2500
