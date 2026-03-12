.text
.global req1
.global req2
.global req3

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
req3:

    bl configUart1

    ldr r0, =nameString
    bl uartSendStr

    b loop


loop3:
    b loop3