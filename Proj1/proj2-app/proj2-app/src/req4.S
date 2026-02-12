.text

@====== NAME REGS ===========
TEMP .req r0
PWM_TMP .req r1
PWM_ADDR .req r2

BRIGHTNESS .req r4
CHANNEL .req r5
TEMP_CHANNEL .req r6

.global req4
.global setWindow
.global setDuty
.global enablePwm

@====== CONSTANTS ===========
.set SW_DATA, 0x41220000

.set PWM_ADRS, 0x43C00000
.set PWM_EN, 0x00

.set PWM_PERIOD, 0x04
.set PWM_WIDTH, 0x08

.set NUM_CHANNELS, 5

@======== MAIN ROUTINE =============
req4:
    b req4Loop

req4Loop:
    bl loadSwitches
    bl disableAll
    bl setWindow
    bl setDuty
    bl enablePwm

    b req4Loop


@======== INPUT SUBROUTINE ========

loadSwitches:
    ldr TEMP,=SW_DATA     @ load switch address once

    ldr TEMP,[TEMP]      @ load switch state
    AND BRIGHTNESS, TEMP, #0xFF   @ bitmask the lower 8 switches into reg 4, reg 4 becomes intensity reg

    lsr CHANNEL, TEMP, #9      @ shift the top 3 switches down to the front
    AND CHANNEL, CHANNEL, #0x7    @ bitmask the bits we just moved

    bx lr

@===== DISABLE ALL SUBROUTINE =====

disableAll:
    push {lr, CHANNEL} @ If you dont store the return reg on the stack, it gets overwritten and lost (learned this the hard way)
                       @ also using my aproach, the channel also needs to be stored on the stack so it can be reinstated after
                       @ the channel reset

    mov CHANNEL, #0
    bl disablePwm
    mov CHANNEL, #1
    bl disablePwm
    mov CHANNEL, #2
    bl disablePwm
    mov CHANNEL, #3
    bl disablePwm
    mov CHANNEL, #4
    bl disablePwm
    mov CHANNEL, #5
    bl disablePwm

    pop {lr, CHANNEL}    @ Restore the link register to the req4 loop
    bx lr

@======== PWM SUBROUTINES ==========

enablePwm:                                         @ effectively PWM_ADRS + (channel num << 4)
    cmp CHANNEL, #NUM_CHANNELS                     @ check if the asserted switches are <= 5, if not, return early
    bxhi lr

    lsl TEMP_CHANNEL,CHANNEL,#4                    @ shift by 4 to multiply to the correct num of bits
    ldr PWM_TMP, =PWM_ADRS

    add TEMP_CHANNEL, PWM_TMP, TEMP_CHANNEL        @ add channel num << 4 to PWM_ADRS, effectively storing PWM channel num EN in r0

    mov PWM_TMP, #1                                @ reuse r1, storing a 1 that will be used to toggle PWM_EN 
    str PWM_TMP,[TEMP_CHANNEL,#PWM_EN]             @ toggle PWM_EN on, by storing a 1 in the defined channel register + offset from constant

    bx lr                                          @ return to caller


disablePwm:                             @ very similar to enablePwm, almost the same, just a 0 this time
    cmp CHANNEL, #NUM_CHANNELS
    bxhi lr                             @ check if the asserted switches are <= 5, if not, return early

    lsl TEMP_CHANNEL, CHANNEL, #4
    ldr PWM_TMP, =PWM_ADRS

    add TEMP_CHANNEL, PWM_TMP, TEMP_CHANNEL

    mov PWM_TMP, #0
    str PWM_TMP,[TEMP_CHANNEL,#PWM_EN] 
    
    bx lr 



setWindow:
    cmp CHANNEL, #NUM_CHANNELS             @ If address is out of bounds, return to caller early
    bxhi lr

    ldr PWM_ADDR,=PWM_ADRS                 @ Load pwm address

    lsl TEMP_CHANNEL,CHANNEL,#4            @ Apply channel offset
    add PWM_ADDR,PWM_ADDR,TEMP_CHANNEL     @ Add the channel offset to the address to access the correct locaton

    ldr PWM_TMP,=2048                        @ Store a hardcoded window value into a temp reg, should be the same as the max value of sw for req4, 2048 for chal1
    str PWM_TMP,[PWM_ADDR, #PWM_PERIOD]     @ Store the temp reg into the width field of the PWM config for a given channel

    bx lr                                  @ Return to caller



setDuty:                                   @ essentially a mirroring of set window, same logic
    cmp CHANNEL,#NUM_CHANNELS              @ If address is out of bounds, return to caller early
    bxhi lr

    ldr PWM_ADDR,=PWM_ADRS                 @ Load pwm address

    lsl TEMP_CHANNEL,CHANNEL,#4            @ Apply channel offset
    add PWM_ADDR,PWM_ADDR,TEMP_CHANNEL     @ Add the channel offset to the address to access the correct locaton

    mov PWM_TMP,BRIGHTNESS                 @ Store brightness in PWM_TMP reg
    str PWM_TMP,[PWM_ADDR, #PWM_WIDTH]     @ Store the brightness indicated by switches into the brightness field of the PWM config for a given channel

    bx lr                                  @ Return to caller