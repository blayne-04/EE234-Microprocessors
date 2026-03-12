.text

.include "C:/Users/beful/Downloads/WSU-SPRING-26/EE234/proj4-app/proj4-app/src/uartDefs.S"

.global req1
.global req2
.global req3
.global req4


@***************************
@          REQ1
@      UART Send Char
@***************************
req1:

    bl configUart1

loop1:

    mov r0, #'W'
    bl uart1SendChar

    b loop1


@***************************
@          REQ2
@   UART Receive + LEDs
@***************************
req2:

    bl configUart1
    b receiveLogic


@***************************
@        REQ3
@   Send Null-Terminated String
@***************************
@ 1. Config UART
@ 2. load name string
@ 3. call sendStr
@ 4. infinite loop
req3:

    bl configUart1

    ldr r0, =nameString
    bl uartSendStr

    b loop3


loop3:
    b loop3


@***************************
@          REQ4
@    Receive UART String
@***************************
@ 1. Config UART
@ 1. Load buffer
@ 2. Set max size
@ 3. Call receiveStr
@ 4. Reload buffer
@ 5. Call sendStr
@ 6. Exit to infinite loop
req4:

    bl configUart1

    ldr r0, =rxBuffer
    mov r1, #128
    bl uartReceiveStr

    ldr r0, =rxBuffer
    bl uartSendStr

loop4:
    b loop4