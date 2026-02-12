.text

@===== CHANGE ======
bright0 .req r7
bright1 .req r8
bright2 .req r9

incr0   .req r10
incr1   .req r11
incr2   .req r12

@====== PWM PARAMS =====
BRIGHTNESS .req r4
CHANNEL    .req r5

.global chal1

chal1: 
    @=====================      Start with some random seed brightnesses, can set these to whatever
    mov bright0,#0  
    mov bright1,#85
    mov bright2,#170            @ 256/3 is 85, split colors into 3rds

    mov incr0,#3
    mov incr1,#5                @ numbers with a small number of factors cause more variation
    mov incr2,#7

    mov CHANNEL,#0
    bl init

loop: 
    add bright0,bright0,incr0   @ Increment each brightness by a different offset
    add bright1,bright1,incr1
    add bright2,bright2,incr2

    and bright0,bright0,#0xFF   @ If we exceed the bitmask, it wraps around nicely, cutting off the upper bits, resetting to 0
    and bright1,bright1,#0xFF
    and bright2,bright2,#0xFF

    @=====================      Red channels 0 & 3
    mov BRIGHTNESS,bright0
    mov CHANNEL,#0
    bl setDuty

    mov CHANNEL,#3
    bl setDuty

    @=====================      Blue channels 1 & 4
    mov BRIGHTNESS,bright1
    mov CHANNEL,#1
    bl setDuty

    mov CHANNEL,#4
    bl setDuty

    @=====================      Green channels 2 & 5
    mov BRIGHTNESS,bright2
    mov CHANNEL,#2
    bl setDuty

    mov CHANNEL,#5
    bl setDuty

    @=====================      Delay for each full combination of 6 channels with different brightnesses
    mov r0,#150
    lsl r0,r0,#18           @ r0 = 150 * 262,144 = 39,321,600 iterations
                            @ delay loop ~3 cycles/iteration => ~117,964,800 CPU cycles
                            @ 667 MHz => ~0.177 seconds per color update (~177 ms)

    bl delay

    b loop
    
init:
    push {lr}

    mov CHANNEL,#0
    bl enablePwm
    bl setWindow
    mov CHANNEL,#1
    bl enablePwm
    bl setWindow
    mov CHANNEL,#2
    bl enablePwm
    bl setWindow
    mov CHANNEL,#3
    bl enablePwm
    bl setWindow
    mov CHANNEL,#4
    bl enablePwm
    bl setWindow
    mov CHANNEL,#5
    bl enablePwm
    bl setWindow

    pop {lr}
    bx lr