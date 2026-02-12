.text
.global req2
.global delay
.global readSwitches

.set SW_DATA, 0x41220000
.set SHIFT_AMT, 18

req2:
    push {lr}               @ Store return address to main, you can do this with a register but the stack is optimal here
	bl led0Toggle
    bl readSwitches

    cmp r0,#0               @ If theres no value on the switches we dont want to underflow, so we compare to zero, and if zero go safetyIncrement
    beq safetyIncrement     @ from there it'll return to main and everything will repeat until a delay is asserted

    lsl r0,r0,#SHIFT_AMT    @ Shift left by 18 so multiply by 262,144, sw val 0->4096 = 1.07billion, 667Hz is 667million cycles/second
                            @ therefore w/ 3 cycles per iteration, we have 667million/3 = 222million iterations/sec, and a delay of 
                            @ 1.07 billion iterations, so 1.07 billion/ 222million = approx 4.81s
	bl delay

    pop {lr}                @ Restore main address so we can make it back on this loop
    bx lr
 
delay:
	subs r0,r0,#1           @ This code is decrementing by 1 from the value on the switches * 2^16 in a loop to delay
	bne delay               @ bne delay will finally be skipped when r0 = 0, and will exit the delay func
	bx lr

readSwitches: 
    ldr r1,=SW_DATA
	ldr r0,[r1]             @ The assembler will replace the psuedoinstruction with the following mov instructions.
	                        @ movw r0,#0 @load lower bits of r0 with 0 movt r0,#0x3000 @load top 16 bits with 0x300
    bx lr

safetyIncrement:
    mov r0,#1
    pop {lr}                @ Restore main address so we can make it back and restart
    bx lr