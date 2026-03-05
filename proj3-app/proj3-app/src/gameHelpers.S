@***************************
@   Function Exports    
@***************************
.text
.global randomNumGen
.global detectEdge
.global msToDigits
.global displayUpdate
.global decrementing
.global incrementing
.global checkZero
.global stopGame
.global resetGame

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
.equ IDLE,    0
.equ RUNNING, 1
.equ STOPPED, 2

@***************************
@  Random Number Generator      
@***************************
@ 1. Truncate to the bottom 13 bits of the seed value
@ 2. Add 1000 (ms) for a 1s minimum
@ 3. Return to caller
randomNumGen:
    ldr temp1, =0x0FFF
    and output, randomSeed, temp1
    add output, output, #1000

    bx lr

@***************************
@    ms -> Digit Convert
@***************************
@ 1. Store input into working reg (temp1)
@ 2. Set counter to 0, (temp2)
@ --- If Input < 1000 ---
@ 3. Store to stack
@ 4. Fall through to 100s place
@ --- Else ---
@ 4. -1000, add to count, loop
@ --- Next ---
@ 5. Same as 1000s for 100s, and 10s
@ 6. Store 1's concurrently with 10s since no need to decrement 
msToDigits:
    mov temp1, input
    mov temp2, #0

thousandsLoop:
    cmp temp1, #1000
    blt storeThousands
    sub temp1, temp1, #1000
    add temp2, temp2, #1
    b thousandsLoop

storeThousands:
    str temp2, [fp, #-16]

    mov temp2, #0

hundredsLoop:
    cmp temp1, #100
    blt storeHundreds
    sub temp1, temp1, #100
    add temp2, temp2, #1
    b hundredsLoop

storeHundreds:
    str temp2, [fp, #-12]
    mov temp2, #0

tensLoop:
    cmp temp1, #10
    blt storeTens
    sub temp1, temp1, #10
    add temp2, temp2, #1
    b tensLoop

storeTens:
    str temp2, [fp, #-8]
    str temp1, [fp, #-4]

    bx lr

@***************************
@   Rising Edge Detector      
@***************************
@ 1. Invert previous button state
@ 2. Detect new presses
@ 3. Store current state
detectEdge:
    mvn btnPressed, prevState
    and btnPressed, btnPressed, input
    mov prevState, input

    bx lr

@***************************
@       Display Update       
@***************************
@ 1. Load registers with the correct digits
@ 2. Shift to offset them for combination
@ 3. combine into r0
@ 4. Pass to displayData
displayUpdate:
    push {lr}
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

    pop {lr}
    bx lr

@***************************
@       Decrementer      
@***************************
@ Aside: It's less mentally taxing to just unroll this code
@ 1. Load temp reg with value from the stack
@ 2. Compare to zero
@ --- If temp /= 0 ---
@ 3. Subtract and Store
@ 4. Return to caller
@ --- Else --- 
@ 5. Else, reset to 9
@ 6. Push onto stack
@ 7. Fall through to next digit
decrementing:
    ldr temp1, [fp, #-4]
    cmp temp1, #0
    bne decD0

    mov temp1, #9
    str temp1, [fp, #-4]

    ldr temp1, [fp, #-8]
    cmp temp1, #0
    bne decD1

    mov temp1, #9
    str temp1, [fp, #-8]

    ldr temp1, [fp, #-12]
    cmp temp1, #0
    bne decD2

    mov temp1, #9
    str temp1, [fp, #-12]

    ldr temp1, [fp, #-16]
    cmp temp1, #0
    bne decD3

    mov temp1, #9
    str temp1, [fp, #-16]

    bx lr

decD0:
    sub temp1, temp1, #1
    str temp1, [fp, #-4]
    bx lr

decD1:
    sub temp1, temp1, #1
    str temp1, [fp, #-8]
    bx lr

decD2:
    sub temp1, temp1, #1
    str temp1, [fp, #-12]
    bx lr

decD3:
    sub temp1, temp1, #1
    str temp1, [fp, #-16]
    bx lr

@***************************
@       Incrementer       
@***************************
@ 1. Load val from stack
@ 2. Increment by 1
@ 3. Compare tempVal to 10
@ --- If Val < 10 ---
@ 4. Store back to stack and exit
@ --- Else ---
@ 5. Reset to 0
@ 6. Store back to stack
@ 7. Fall through to next digit
incrementing:
    ldr temp1, [fp, #-4]
    add temp1, temp1, #1
    cmp temp1, #10
    blt incD0

    mov temp1, #0
    str temp1, [fp, #-4]

    ldr temp1, [fp, #-8]
    add temp1, temp1, #1
    cmp temp1, #10
    blt incD1

    mov temp1, #0
    str temp1, [fp, #-8]

    ldr temp1, [fp, #-12]
    add temp1, temp1, #1
    cmp temp1, #10
    blt incD2

    mov temp1, #0
    str temp1, [fp, #-12]

    ldr temp1, [fp, #-16]
    add temp1, temp1, #1
    cmp temp1, #10
    blt incD3

    mov temp1, #0
    str temp1, [fp, #-16]

    bx lr

incD0:
    str temp1, [fp, #-4]
    bx lr

incD1:
    str temp1, [fp, #-8]
    bx lr

incD2:
    str temp1, [fp, #-12]
    bx lr

incD3:
    str temp1, [fp, #-16]
    bx lr

@***************************
@       Check Zero      
@***************************
@ 1. Rip data off the stack
@ 2. OR it all together
@ 3. Compare result to zero
@ --- If result = 0 ---
@ 4. Set output to 1
@ 5. Return to caller
@ --- Else ---
@ 4. Set output to 0
@ 5. Return to caller
checkZero:
    ldr temp1, [fp, #-4]
    ldr temp2, [fp, #-8]
    orr temp1, temp1, temp2

    ldr temp2, [fp, #-12]
    orr temp1, temp1, temp2

    ldr temp2, [fp, #-16]
    orr temp1, temp1, temp2

    cmp temp1, #0
    moveq output, #1
    movne output, #0

    bx lr

@***************************
@        Stop Game
@***************************
@ 1. Update gameState to stoppedState
@ 2. Return to caller
stopGame:
    mov gameState, #STOPPED

    bx lr

@***************************
@        resetGame
@***************************
@ 1. Set temp1 to zero
@ 2. Store zeroes on stack
@ 3. Reset sign flag
@ 4. Reset game state
@ 5. Update display
@ 6. Return to caller
resetGame:
    push {lr}
    mov temp1, #0
    str temp1, [fp, #-4]
    str temp1, [fp, #-8]
    str temp1, [fp, #-12]
    str temp1, [fp, #-16]

    mov signFlag, #0
    mov gameState, #IDLE

    bl displayUpdate
    
    pop {lr}
    bx lr