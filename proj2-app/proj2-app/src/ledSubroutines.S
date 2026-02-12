.text
.global led0On
.global led0Off
.global led0Toggle

.equ LED_CTL, 0x41210000            @led address
.equ LED_ACTIVE_MASK, 0x00000001    @adjustable bitmask

led0On:
	ldr r1,=LED_CTL                 @tie r1 to address
	ldr r0, [r1]	                @get current value
	orr r0,r0,#LED_ACTIVE_MASK	    @set the first bit (don't affect other bits)
	str r0, [r1]	                @write back to LED_DATA
	bx lr		                    @return from subroutine

led0Off: 
    ldr r1, =LED_CTL                @tie r1 to led address
    ldr r0, [r1]                    @get current value of led
    bic r0,r0,#LED_ACTIVE_MASK      @bit clear, i.e reset
    str r0, [r1]                    @store the result, (a zero)
    bx lr                           @exit subroutine

led0Toggle:
    ldr r1,=LED_CTL
    ldr r0, [r1]                    @get current value of LED
    eor r0,r0,#LED_ACTIVE_MASK      @toggle LED state
    str r0, [r1]                    @store that back in LED_DATA
    bx lr                           @exit subroutine