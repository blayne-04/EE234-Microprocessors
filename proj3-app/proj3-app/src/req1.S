.text
.global req1

@ Important Convention: r0 is a return reg in the context of this program, r0->r3 are used as inputs and outputs
@ For "Display Data" the program expects the digit indicator in r0 and the value in r1

req1:
    bl initSevenSeg
    b req1Loop

req1Loop:
    bl readSwitches
    mov r1, r0      @ store switch data in r1

    bl readButtons
    mov r2, r0      @ store button data in r2

    lsl r2, r2, #12   @ shift btn data left 12
    orr r1, r1, r2    @ combine r2 with r1 

    bl displayData  @ call display function, this will use r1 to display values

    b req1Loop