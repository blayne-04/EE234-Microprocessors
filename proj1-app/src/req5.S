.text
.global req5

.equ LED_CTL, 0x41210000
.set SW_DATA, 0x41220000

req5:
	ldr r1,=SW_DATA	
	ldr r2,=LED_CTL
	

req5_loop:
    @continuously load switch registers
    ldr r0,[r1]         @load r0 from the value at the location of r1, i.e the switches
    and r0, r0, #0xF    @bitmask to only use lower 4 bits of r1;

    @condition 1: sw = 0->3 => led 0
    cmp r0,#4
    blt req5_led0

    @condition 2: sw = 4->7 => led 1
    cmp r0,#8
    blt req5_led1

    @condition 3: sw = 8->11 => led 2
    cmp r0,#12
    blt req5_led2

    @condition 4: sw = 12->15 => led 3, this is just an else
    b req5_led3

req5_led0:		@if sw is in range 0-> drive led 0, return to loop
	mov r4,#0x1
	str r4,[r2]
	b req5_loop

req5_led1:		@if sw is in range 0-> drive led 1, return to loop
	mov r4,#0x2
	str r4,[r2]
	b req5_loop

req5_led2:		@if sw is in range 0-> drive led 2, return to loop
	mov r4,#0x4
	str r4,[r2]	
	b req5_loop

req5_led3:		@if sw is in range 0-> drive led 3, return to loop
	mov r4,#0x8
	str r4,[r2]
	b req5_loop

.end