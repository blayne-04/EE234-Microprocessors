.text
.global req3

@ .equ DELAY_VAL, 805000 @ not necessary while using TTC module

req3:
    bl initSevenSeg         @ Enable seven seg display
    bl TTC                  @ Enable TTC module

    push {fp, lr}
    mov  fp, sp
    sub  sp, sp, #28        @ allocate 7 digits

    mov r0, #0              @ initialize digits to 0
    str r0, [fp, #-4]
    str r0, [fp, #-8]
    str r0, [fp, #-12]
    str r0, [fp, #-16]
    str r0, [fp, #-20]
    str r0, [fp, #-24]
    str r0, [fp, #-28]

    mov r4, #0              @ previous button state
    mov r7, #0              @ running state (0 = idle)

req3Loop:
    bl  readButtons
    and r5, r0, #0x7    @ bitmask for btn 0, 1 and 2

    @ Rising edge detection
    mvn r6, r4          @ r6 is inverse of previous button state
    and r6, r6, r5      @ if previous state was a 0, and r5 is asserted, r6 = 1
    mov r4, r5          @ store btn state in r4

    tst r6, #4          @ if start/stop btn asserted
    eorne r7, r7, #1

    cmp r7, #1          @ if r7 indicates the program is running, branch to running
    beq running

    @ else we continue here, just listening
    cmp r6, #0          @ if no btn asserted
    beq req3Loop

    cmp r6, #2
    beq clrHandler

    cmp r6, #1
    beq increment

    b req3Loop

running:
    @ ldr r8, =DELAY_VAL   @ These 2 lines are an artifact from the primitive decrement loop
    @ bl delay

    bl readSwitches
    and r0, r0, #0x7        @ get switch value 0–7
    mov r1, #108            @ base interval for ~1ms
    lsl r0, r1, r0          @ r0 = 108 << switch_value

    bl ttc0_set_itvl        @ update TTC interval based on sw value
    bl ttc0_poll            @ Call polling function to await variable time using the TTC module
    b increment

@ delay:
@ 	subs r8,r8,#1           @ This code is decrementing by 1 from the value on the switches * 2^16 in a loop to delay
@	bne delay               @ bne delay will finally be skipped when r0 = 0, and will exit the delay func
@	bx lr

@***************************
@        Clear Disp        
@***************************
clrHandler:

    mov r0, #0
    str r0, [fp, #-4]
    str r0, [fp, #-8]
    str r0, [fp, #-12]
    str r0, [fp, #-16]
    str r0, [fp, #-20]
    str r0, [fp, #-24]
    str r0, [fp, #-28]

    bl clearDisp
    b updateDisplay

@***************************
@    Ripple Increment        
@***************************
increment:
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

    @***************************
    @         Data 4            
    @***************************
    ldr r1, [fp, #-20]
    add r1, r1, #1
    cmp r1, #10
    blt storeD4Done

    mov r1, #0
    str r1, [fp, #-20]

    @***************************
    @         Data 5            
    @***************************
    ldr r1, [fp, #-24]
    add r1, r1, #1
    cmp r1, #10
    blt storeD5Done

    mov r1, #0
    str r1, [fp, #-24]

    @***************************
    @         Data 6            
    @***************************
    ldr r1, [fp, #-28]
    add r1, r1, #1
    cmp r1, #10
    blt storeD6Done

    mov r1, #0
    str r1, [fp, #-28]

    b updateDisplay


@***************************
@        Store Funcs       
@***************************
@ All funcs follow the algorithm
@ 1. Store r1 into stack
@ 2. Update Display

storeD6Done:
    str r1, [fp, #-28]
    b updateDisplay

storeD5Done:
    str r1, [fp, #-24]
    b updateDisplay

storeD4Done:
    str r1, [fp, #-20]
    b updateDisplay

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
@ 1. Read sw12 to determine if upper digits or lower digits are displayed
@ 2. Branch to the appropriate load function
@ 3. Load registers with the correct digits
@ 4. Shift to offset them for combination
@ 5. combine into r0
@ 6. Pass to displayData

updateDisplay:
    bl readSwitches
    and r8, r0, #0x800  @ bitmask for 12th switch
    cmp r8, #0

    bleq loadLower
    blne loadUpper

    lsl r3, r3, #12
    lsl r2, r2, #8
    lsl r1, r1, #4

    orr r0, r0, r1
    orr r0, r0, r2
    orr r0, r0, r3

    mov r1, r0
    bl displayData

    b req3Loop

loadLower:

    ldr r0, [fp, #-4]      @ D0
    ldr r1, [fp, #-8]      @ D1
    ldr r2, [fp, #-12]     @ D2
    ldr r3, [fp, #-16]     @ D3

    bx lr

loadUpper: 
    ldr r0, [fp, #-16]      @ D0
    ldr r1, [fp, #-20]      @ D1
    ldr r2, [fp, #-24]     @ D2
    ldr r3, [fp, #-28]     @ D3

    bx lr