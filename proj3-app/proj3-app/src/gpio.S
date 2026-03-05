.text
.global readSwitches
.global readButtons

.set SW_DATA, 0x41220000
.set BTN_DATA, 0x41200000

readSwitches:
    ldr r1, =SW_DATA   @ r1 = address
    ldr r0, [r1]       @ r0 = switch value
    mov r2,#0xFFF
    and r0, r0, r2     @ mask to 12 bits

    bx lr

readButtons:
    ldr r3, =BTN_DATA
    ldr r0, [r3]
    and r0, r0, #0xF   @ mask to 4 bits

    bx lr