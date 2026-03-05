.text
.global initRTM

@***************************
@   Register Definitions
@***************************
input .req r0
output .req r1
arg1 .req r2
arg2 .req r3

gameState .req r4
randomSeed .req r5
timerVal .req r6
signFlag .req r7
prevState .req r8
btnPressed .req r9

temp1 .req r10
temp2 .req r12

@***************************
@        Constants
@***************************
.equ IDLE, 0
.equ WAIT_RANDOM, 1
.equ TIMING, 2
.equ STOPPED, 3

.equ BTN_START, 1
.equ BTN_REACT, 2

@***************************
@      Initialization
@***************************
@ 1. Enable seven seg display
@ 2. Enable TTC module
@ 3. Setup stack frame
@ 4. Allocate storage for digits
@ 5. Initialize registers
@ 6. Clear display and LED
initRTM:
    bl initSevenSeg
    bl TTC

    push {fp, lr}
    mov fp, sp
    sub sp, sp, #16

    mov gameState, #IDLE
    mov randomSeed, #0
    mov prevState, #0

    bl led0Off
    bl resetGame

@***************************
@       Main Loop
@***************************
@ 1. Increment random seed
@ 2. Read button input
@ 3. Detect rising edges
@ 4. Run FSM
gameLoop:

    add randomSeed, randomSeed, #1

    bl readButtons
    and r0, r0, #0x7
    bl detectEdge

    cmp gameState, #IDLE
    beq idleState

    cmp gameState, #WAIT_RANDOM
    beq waitRandomState

    cmp gameState, #TIMING
    beq timingState

    cmp gameState, #STOPPED
    beq stoppedState

    b gameLoop

@***************************
@           IDLE
@***************************
@ 1. Wait for start button
@ --- If asserted ---
@ 2. Generate random delay
@ 3. Store delay in timerVal
@ 4. Clear display
@ 5. Make sure LED is off
@ 6. Transition to random waittime 
idleState:

    tst btnPressed, #BTN_START
    beq gameLoop

    bl randomNumGen
    mov timerVal, output

    bl led0Off

    mov input, #0
    bl msToDigits
    bl displayUpdate

    mov gameState, #WAIT_RANDOM

    b gameLoop

@***************************
@        WAIT_RANDOM
@***************************
@ 1. Poll TTC (1ms)
@ 2. Decrement delay counter
@ --- If delay not zero ---
@ 3. Continue waiting
@ --- Else ---
@ 4. Turn LED on
@ 5. Clear timer
@ 6. Transition to TIMING
waitRandomState:

    bl ttc0_poll

    subs timerVal, timerVal, #1
    bgt gameLoop

    bl led0On

    mov timerVal, #0

    mov input, timerVal
    bl msToDigits
    bl displayUpdate

    mov gameState, #TIMING

    b gameLoop

@***************************
@          TIMING
@***************************
@ 1. Poll TTC (1ms)
@ 2. Increment timer
@ 3. Update display
@ 4. Check react button
@ --- If pressed ---
@ 5. Turn LED off
@ 6. Stop timer
@ 7. Move to STOPPED
timingState:

    bl ttc0_poll

    add timerVal, timerVal, #1

    mov input, timerVal
    bl msToDigits
    bl displayUpdate

    tst btnPressed, #BTN_REACT
    beq gameLoop

    bl led0Off

    mov gameState, #STOPPED

    b gameLoop

@***************************
@          STOPPED
@***************************
@ 1. Wait for start button
@ --- If pressed ---
@ 2. Reset game
@ 3. Return to FSM
stoppedState:

    tst btnPressed, #BTN_START
    beq gameLoop

    bl resetGame
    bl led0Off

    b gameLoop