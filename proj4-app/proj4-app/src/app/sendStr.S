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
@ 1. load string from r0 into r4
@ 2. load byte
@ 3. compare byte to Null, if true, terminate
@ 4. else call send char 
@ 5. advance string ptr
@ 6. repeat steps 2-5

uartSendStr:
    push {r4}
    mov r4, r0

nextChar:

    ldrb r0, [r4]
    cmp r0, #0
    beq terminate

    bl uart1SendChar

    add r4, r4, #1
    b nextChar

terminate:
    pop {r4}
    bx lr