.text
.global req4

.equ LED_CTL, 0x41210000
.set SW_DATA, 0x41220000


req4:
	ldr r1,=SW_DATA	
	ldr r2,=LED_CTL
	movw r3,#0xFFF 	@store 0xFFF in the lower 16 bits of a register since it's 12 bits wide

req4_loop:
	ldr r0,[r1]     @load switch value, (*r1) ->r0
	cmp r0,#21      @compare switch value with 21
	beq req4_led1  	@branch to led1 if == 21
    cmp r0,r3		@compare sw bus to sw bus value when all 12 sw are en
    beq req4_led2  	@branch to led 2 if all sw en

req4_led_off:		@if led is off, set all lights off, return to loop
	mov r0,#0x0
	str r0,[r2]
	b req4_loop

req4_led1:		@if sw == 21 set 1st led to on, return to loop
	mov r0,#0x1
	str r0,[r2]
	b req4_loop

req4_led2:		@ if all sw are on, turn on led2, return to loop
	mov r0,#0x2
	str r0,[r2]	
	b req4_loop

.end