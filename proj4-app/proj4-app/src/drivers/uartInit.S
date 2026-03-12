.include "../../include/uart_defs.S"

.text
.global configUart1

@***************************
@      Config UART1
@***************************
configUart1:

    push {lr}
    bl resetUart1

@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@       Configure mode reg
@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    ldr r1, =UART1_MR
    mov r0, #SETUP_OPCODE
    str r0, [r1]

@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@       Config Baud Rate
@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@ BAUDGEN = 0x7C
@ BAUDDIV = 6
@ ~115200 baud

    ldr r1, =UART1_BAUDGEN
    mov r0, #0x7C
    str r0, [r1]

    ldr r1, =UART1_BAUDDIV
    mov r0, #6
    str r0, [r1]

@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@       Enable TX & RX
@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    ldr r1, =UART1_CR

    mov r0, #4
    orr r0, r0, #16
    str r0, [r1]

    pop {lr}
    bx lr


@***************************
@       Reset UART1
@***************************

resetUart1:

    ldr r1, =UART1_CR

    mov r0, #3
    str r0, [r1]

resetLoop:

    ldr r2, [r1]
    teq r2, r0
    bne resetLoop

    bx lr