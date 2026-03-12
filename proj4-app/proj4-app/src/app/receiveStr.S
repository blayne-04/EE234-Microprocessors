.text
.global uartReceiveStr

.include "C:/Users/beful/Downloads/WSU-SPRING-26/EE234/proj4-app/proj4-app/src/uartDefs.S"

@***************************
@ Receive String From UART
@***************************
@ PARAMETERS:
@ r0 = buffer address
@ r1 = max length


@***************************
@       Initialization 
@***************************
@ 1. preserve r4, r5, r6, lr for caller
@ 2. assert r4 = buffer pointer
@ 3. assert r5 = max length
@ 4. assert r6 = index

uartReceiveStr:
    push {r4, r5, r6, lr}
    mov r4, r0
    mov r5, r1
    mov r6, #0

@***************************
@       Core Loop
@***************************
@ 1. call getChar, this func returns char in r0
@ 2. check if char we got is == '\n'
@ 3. if so, terminate
@ 4. else check if we reached max length
@ 5. if so, terminate
@ 6. else store char byte into r0
@ 7. walk buffer ptr and index + 1
@ 8. repeat steps 1-7

receiveLoop:

    bl uart1GetChar

    cmp r0, #'\n'
    beq terminate
    cmp r0, #'\r'
    beq terminate

    cmp r6, r5
    bge terminate

    strb r0, [r4]

    add r4, r4, #1
    add r6, r6, #1

    b receiveLoop

@***************************
@           Exit
@***************************
@ 1. append '\0'
@ 2. restore r4, r5, r6, lr for caller
@ 3. return to caller

terminate:
    mov r0, #0
    strb r0, [r4]
    pop {r4, r5, r6, lr}


    bx lr