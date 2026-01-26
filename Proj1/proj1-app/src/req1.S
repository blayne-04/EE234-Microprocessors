.text
.global req1

@define const
.equ LED_CTL, 0x41210000
.set SW_DATA, 0x41220000

@set and equ are equivalent and can be used as such

req1:
	ldr r1,=SW_DATA	@load switch address from constant
	ldr r2,=LED_CTL	@load LED address from constant
req1_loop:
	ldr r0,[r1]	@load switch value *r1 ->r0
	str r0,[r2]	@store value to led register *r2 <-r0
	b req1_loop	@go back to "loop"

.end