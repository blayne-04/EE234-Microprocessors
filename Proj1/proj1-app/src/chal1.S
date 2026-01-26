.text
.global chal1

@define const
.set LED_CTL, 0x41210000
.set SW_DATA, 0x41220000
.set BTN_DATA, 0x41200000

chal1:
	ldr r1,=SW_DATA
	ldr r2,=BTN_DATA
    ldr r3,=LED_CTL

chal1_loop:
    @load btn and sw data into registers
    ldr r4,[r1]
    ldr r5,[r2]

    @apply a bit mask so only lower 4 switches remain
    and r4,r4,#0xF
    and r5,r5,#0xF

    @xor op with btn and sw regs and drive led output with the result
    eor r0,r4,r5
    str r0,[r3]
    
    @continue looping
    b chal1_loop