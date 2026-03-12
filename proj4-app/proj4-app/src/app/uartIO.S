.include "C:/Users/beful/Downloads/WSU-SPRING-26/EE234/proj4-app/proj4-app/src/uartDefs.S"

.text
.global uart1SendChar
.global uart1GetChar


@***************************
@      Send UART Char
@***************************
@ r0 = ASCII character

uart1SendChar:

    ldr r1, =UART1_SR

awaitSend:

    ldr r2, [r1]

    tst r2, #16
    bne awaitSend

    ldr r1, =UART1_FIFO
    str r0, [r1]

    bx lr


@***************************
@      Get UART Char
@***************************

uart1GetChar:

    ldr r1, =UART1_SR

awaitReceive:

    ldr r2, [r1]

    tst r2, #2
    bne awaitReceive

    ldr r1, =UART1_FIFO
    ldrb r0, [r1]

    bx lr