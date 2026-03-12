.text
.global receiveLogic

@***************************
@      LED Bit Masks
@***************************

.equ LED0, 1
.equ LED1, 2
.equ LED2, 4
.equ LED3, 8


receiveLogic:

    bl uart1GetChar


@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@           Check 'a'
@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
checkA:

    cmp r0, #'a'
    bne checkB

    mov r2, #LED0
    bl ledToggle

    mov r0, #'0'
    bl uart1SendChar
    b receiveLogic


@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@           Check 'b'
@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

checkB:

    cmp r0, #'b'
    bne checkC

    mov r2, #LED1
    bl ledToggle

    mov r0, #'1'
    bl uart1SendChar
    b receiveLogic


@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@           Check 'c'
@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

checkC:

    cmp r0, #'c'
    bne checkD

    mov r2, #LED2
    bl ledToggle

    mov r0, #'2'
    bl uart1SendChar
    b receiveLogic


@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
@           Check 'd'
@~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

checkD:

    cmp r0, #'d'
    bne receiveLogic

    mov r2, #LED3
    bl ledToggle

    mov r0, #'3'
    bl uart1SendChar
    b receiveLogic