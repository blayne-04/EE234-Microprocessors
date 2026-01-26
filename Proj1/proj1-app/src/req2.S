.text
.global req2

@define const
.equ LED_CTL, 0x41210000
.set SW_DATA, 0x41220000
    
req2:
	ldr r1,=SW_DATA	   @load switch address from constant
	ldr r2,=LED_CTL	   @load LED address from constant

req2_loop:
	ldr r0,[r1]    @load switch value *r1 ->r0
    mvn r0, r0     @invert r0 before its stored in the LED, making led active low
	str r0,[r2]    @store value to led register *r2 <-r0
	b req2_loop	   @continue looping

.end