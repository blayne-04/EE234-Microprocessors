.text
.global req3
.global setPwmDelay

.set SW_DATA, 0x41220000
.set PWM_DELAY_MODIFIER, 10
.set PWM_WINDOW_CONST, 0x1000       @ 1 more bit than the switches can possibly provide, prevents the case where all sw asserted turns leds off

req3:
    push {r4,r5,lr}                 @ store values we may want to save on the stack so we dont lose them

    bl setPwmDelay                  @ set delays using switches

    cmp r5,#0
    beq pwmOff

    bl led0On                       @ this block of code turns the led on, stores the indicated ontime in r0
    mov r0, r5                      @ then calls delay, which uses r0 as its input parameter
    bl delay

    bl led0Off                      @ this block of code turns the led off, stores (window - ontime)
    sub r0,r4,r5                    @ then calls delay, which uses r0 as its input parameter
    bl delay

    pop {r4,r5,lr}                  @ pop back off the values we stored at the beginning so they can be used in the rest of the program

    bx lr


setPwmDelay:
    ldr r1, =SW_DATA                  @ load switch data
    ldr r0, [r1]

    mov r4,#PWM_WINDOW_CONST          @ store window constant in r4
    mov r5,r0                         @ extract 12 bit switch value from switches and store in r5

    lsl r4,r4,#PWM_DELAY_MODIFIER     @ shift to convert to a substantial delay, note approx 222k iterations/cycle and 
    lsl r5,r5,#PWM_DELAY_MODIFIER     @ {sw input} * 2^{PWM_DELAY_MODIFIER} cycles

    bx lr                             @ return to calling function

pwmOff:                             @ if the PWM ontime isnt asserted, just keep going back to main and looping til it is
    bl led0Off

    pop {r4,r5,lr}                  @ pop back off the values we stored at the beginning so they can be used in the rest of the program

    bx lr