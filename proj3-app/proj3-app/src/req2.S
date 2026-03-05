.text
.global req2

req2:
    bl initSevenSeg
    push {fp, lr}
    mov  fp, sp
    sub  sp, sp, #16        @ allocate 4 digits

    mov r0, #0              @ initialize digits to 0
    str r0, [fp, #-4]
    str r0, [fp, #-8]
    str r0, [fp, #-12]
    str r0, [fp, #-16]

    mov r4, #0              @ previous button state

req2Loop:

    bl  readButtons
    and r5, r0, #0x3    @ bitmask for btn 0 and 1

    @ Rising edge detection
    mvn r6, r4          @ r6 is inverse of previous button state
    and r6, r6, r5      @ if previous state was a 0, and r5 is asserted, r6 = 1
    mov r4, r5          @ store btn state in r4

    cmp r6, #0          @ if no btn asserted
    beq req2Loop

    cmp r6, #2
    beq clrHandler

    cmp r6, #1
    beq incHandler

    b req2Loop

@***************************
@        Inc Switch        
@***************************
incHandler:
    @ this func is basically a switch statement: switch(channel)
    @ if r0, 0 -> channel 0, if r0, 1 -> channel 1, and so on.

    bl readSwitches
    and r0, r0, #0x3

    cmp r0, #0
    beq incD0Ripple

    cmp r0, #1
    beq incD1

    cmp r0, #2
    beq incD2

    b   incD3

@***************************
@        Clear Disp        
@***************************
clrHandler:

    mov r0, #0
    str r0, [fp, #-4]
    str r0, [fp, #-8]
    str r0, [fp, #-12]
    str r0, [fp, #-16]

    bl clearDisp
    b updateDisplay

@***************************
@    Ripple Increment        
@***************************
incD0Ripple:
    @ All entries follow the algorithm
    @ 1. load register from stack
    @ 2. add 1
    @ 3. compare to 10
    @ 4. if less than 10 call exit func 
    @ 5. otherwise reset to 0
    @ 6. push back to stack

    @***************************
    @         Data 0            
    @***************************

    ldr r1, [fp, #-4]
    add r1, r1, #1
    cmp r1, #10
    blt storeD0Done

    mov r1, #0
    str r1, [fp, #-4]

    @***************************
    @         Data 1            
    @***************************
    ldr r1, [fp, #-8]
    add r1, r1, #1
    cmp r1, #10
    blt storeD1Done

    mov r1, #0
    str r1, [fp, #-8]

    @***************************
    @         Data 2            
    @***************************
    ldr r1, [fp, #-12]
    add r1, r1, #1
    cmp r1, #10
    blt storeD2Done

    mov r1, #0
    str r1, [fp, #-12]

    @***************************
    @         Data 3            
    @***************************
    ldr r1, [fp, #-16]
    add r1, r1, #1
    cmp r1, #10
    blt storeD3Done

    mov r1, #0
    str r1, [fp, #-16]

    b updateDisplay


@***************************
@     Single Digit Inc     
@***************************
@ All entries follow the algorithm
@ 1. Load from Stack
@ 2. add 1
@ 3. compare to 10
@ 4. reset to 0 if >= 10
@ 5. otherewise store updated val into stack
@ 6. update display

incD1:
    ldr r1, [fp, #-8]
    add r1, r1, #1
    cmp r1, #10
    movge r1, #0
    str r1, [fp, #-8]
    b updateDisplay

incD2:
    ldr r1, [fp, #-12]
    add r1, r1, #1
    cmp r1, #10
    movge r1, #0
    str r1, [fp, #-12]
    b updateDisplay

incD3:
    ldr r1, [fp, #-16]
    add r1, r1, #1
    cmp r1, #10
    movge r1, #0
    str r1, [fp, #-16]
    b updateDisplay

@***************************
@        Store Funcs       
@***************************
@ All funcs follow the algorithm
@ 1. Store r1 into stack
@ 2. Update Display

storeD3Done:
    str r1, [fp, #-16]
    b updateDisplay

storeD2Done:
    str r1, [fp, #-12]
    b updateDisplay

storeD1Done:
    str r1, [fp, #-8]
    b updateDisplay

storeD0Done:
    str r1, [fp, #-4]
    b updateDisplay

@***************************
@       Disp Update       
@***************************
@ 1. Load all register values from stack
@ 2. Shift to offset them for combination
@ 3. combine into r0
@ 4. Pass to displayData

updateDisplay:

    ldr r0, [fp, #-4]      @ D0
    ldr r1, [fp, #-8]      @ D1
    ldr r2, [fp, #-12]     @ D2
    ldr r3, [fp, #-16]     @ D3

    lsl r3, r3, #12
    lsl r2, r2, #8
    lsl r1, r1, #4

    orr r0, r0, r1
    orr r0, r0, r2
    orr r0, r0, r3

    mov r1, r0
    bl displayData

    b req2Loop