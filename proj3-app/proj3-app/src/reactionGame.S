.text
.global initGame

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

.equ IDLE,    0
.equ RUNNING, 1
.equ STOPPED, 2

.equ BTN_STARTSTOP, 1
.equ BTN_RESET, 2

.equ MINUS_SIGN, 0xA

@***************************
@      Initialization
@***************************
@ 1. Enable seven seg display
@ 2. Enable TTC module
@ 3. Setup stack frame
@ 4. Allocate storage for digits
@ 5. Initialize registers
@ 6. Clear display
initGame:
    bl initSevenSeg
    bl TTC
    
    push {fp, lr}
    mov fp, sp
    sub sp, sp, #16

    mov gameState, #IDLE
    mov randomSeed, #0
    mov prevState, #0
    mov signFlag, #0

    mov temp1, #0
    str temp1, [fp, #-4]
    str temp1, [fp, #-8]
    str temp1, [fp, #-12]
    str temp1, [fp, #-16]

    bl displayUpdate

@***************************
@       Main Loop
@***************************
@ 1. Increment random seed
@ 2. Read button input
@ 3. Detect button edges
@ 4. Use FSM
gameLoop:
    add randomSeed, randomSeed, #1

    bl readButtons
    and r0, r0, #0x7
    bl detectEdge

    cmp gameState, #IDLE
    beq idleState

    cmp gameState, #RUNNING
    beq runningState

    cmp gameState, #STOPPED
    beq stoppedState

    b gameLoop

@***************************
@         IDLE
@***************************
@ 1. Wait for start button
@ --- If pressed ---
@ 2. Generate random number
@ 3. Convert to digits
@ 4. Set running state
@ 5. Update display
idleState:
    tst btnPressed, #BTN_STARTSTOP
    beq gameLoop

    bl randomNumGen
    mov input, output
    bl msToDigits

    mov signFlag, #0
    mov gameState, #RUNNING

    bl displayUpdate

    b gameLoop

@***************************
@       RUNNING
@***************************
@ 1. Poll TTC (1 ms delay)
@ 2. Check sign flag
@ --- If flag = 0 ---
@ 3. Countdown
@ --- Else ---
@ 4. Count up
runningState:
    bl ttc0_poll

    cmp signFlag, #0
    beq doCountdown

@***************************
@       After Zero
@***************************
@ 1. Increment counter
@ 2. Place minus sign in MS digit
@ 3. Continue to endStep
afterZero:
    bl incrementing

    mov temp1, #MINUS_SIGN
    str temp1, [fp, #-16]

    b endStep

@***************************
@       Countdown
@***************************
@ 1. Decrement counter
@ 2. Check if zero reached
@ --- If zero ---
@ 3. Assert sign flag
doCountdown:
    bl decrementing

    bl checkZero
    cmp output, #1
    bne endStep

    mov signFlag, #1

@***************************
@        End Step
@***************************
@ 1. Update display
@ 2. Check stop button
@ 3. Return to FSM
endStep:
    bl displayUpdate

    tst btnPressed, #BTN_STARTSTOP
    blne stopGame

    b gameLoop

@***************************
@          STOPPED
@***************************
@ 1. Wait for reset button
@ 2. Return to FSM
stoppedState:
    tst btnPressed, #BTN_RESET
    blne resetGame

    b gameLoop