.data
.global nameString

@***************************
@         DATA
@***************************

nameString:
    .byte 'B','l','a','y','n','e',0


.text
.global uartSendStr

@***************************
@   Send Null-Terminated String
@***************************

uartSendStr:

    mov r1, r0

nextChar:

    ldrb r0, [r1]
    cmp r0, #0
    beq done

    bl uart1SendChar

    add r1, r1, #1
    b nextChar

done:
    bx lr