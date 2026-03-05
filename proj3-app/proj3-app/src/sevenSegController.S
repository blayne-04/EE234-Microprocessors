.text
.global initSevenSeg
.global displayData
.global clearDisp

.equ SVN_SEG_CNTL, 0x43C10000  @ Seven seg enable
.equ SVN_SEG_DATA, 0x43C10004  @ Data bits span from 0x43C10004 to 0x43C10010 each byte is 1 char

initSevenSeg:
    ldr r0,=SVN_SEG_CNTL
    mov r1,#1
    str r1,[r0]

    bx lr

displayData:    @ r1 = 16 bit packed data register
    push {r4, r5, r6, r7, lr}

    @ Extract digits
    and r3, r1, #0xF        @ digit 0 (sw)

    lsr r4, r1, #4
    and r4, r4, #0xF        @ digit 1 (sw >> 4)
    
    lsr r5, r1, #8
    and r5, r5, #0xF        @ digit 2 (sw >> 8)

    lsr r6, r1, #12
    and r6, r6, #0xF        @ digit 3 (btn)

    @ Shift data so it can be combined
    lsl r6, r6, #24     @ (digit 3 << 24)
    lsl r5, r5, #16     @ (digit 2 << 16)
    lsl r4, r4, #8      @ (digit 1 << 8)
    @ digit 0 maintains it's location

    @ Load the shifted data into a single temp reg
    mov r7, r3          @ load digit 0
    orr r7, r7, r4      @ load digit 1
    orr r7, r7, r5      @ load digit 2
    orr r7, r7, r6      @ load digit 3

    @ load SVN_SEG_DATA with the built register (r7)
    ldr r0, =SVN_SEG_DATA
    str r7, [r0]

    pop {r4, r5, r6, r7, lr}
    bx lr

    clearDisp:
    	ldr r1,=SVN_SEG_DATA
	    mov r0,#0
	    str r0,[r1]
	    bx lr