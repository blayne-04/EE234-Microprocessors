.text
.global ledOn
.global ledOff
.global ledToggle

.equ LED_CTL, 0x41210000

@***************************
@       LED FUNCTIONS
@***************************
@ Note: All funcs expect bitmask is 
@ stored in r2 before calls

ledOn:
	ldr r1, =LED_CTL
	ldr r0, [r1]
	orr r0, r0, r2
	str r0, [r1]
	bx lr

ledOff: 
    ldr r1, =LED_CTL
    ldr r0, [r1]
    bic r0, r0, r2
    str r0, [r1]
    bx lr

ledToggle:
    ldr r1, =LED_CTL
    ldr r0, [r1]
    eor r0, r0, r2
    str r0, [r1]
    bx lr