.text
.global TTC
.global ttc0_poll
.global ttc0_set_itvl

@ Addresses
.equ TTC0_BASE, 	0xF8001000	@ ttc0 clock control reg
.equ TTC0_CLKCTL_OFF,	0x0
.equ TTC0_CNTCTL_OFF,	0xC		@ ttc0 count 1 control
.equ TTC0_ITVL_OFF,	0x24		@ ttc0 interval 1 reg
.equ TTC0_IER_OFF,	0x60		@ ttc0 IER (interrupt enable)
.equ TTC0_ISR_OFF,	0x54		@ ttc0 ISR (interrupt status)
.equ PRESCALE_VAL, 9            @ prescale value for approx 108.5 KHz
.equ INTERVAL_VAL, 108          @ interval value for approx 1ms assuming 108.5 KHz


@ Constants used to setup timer
.equ div_mask,	0xFFFFFFE1 	@ mask to clear only bits 4:1
.equ cnt_dis_rst, 0x11		@ settings to disable and reset counter
.equ cnt_en_itvl, 0x22		@ settings to enable counter in interval mode
.equ intvl_int_en, 0x1		@ settings to enable interrupt signal so it can be read

TTC:
    push {lr}

    bl ttc0_reset       @ reset and clear TTC module

    bl ttc0_clk_conf    @ configure TTC0 to use prescale and pclk

    mov r0, #PRESCALE_VAL   @ set r0 to a prescale value
    bl ttc0_set_div         @ call the clock divider to use the prescale value

    mov r0, #INTERVAL_VAL   @ set r0 to the interval value
    bl ttc0_set_itvl        @ set the interval

    bl ttc0_en_itvl_int     @ enable the interval flag bit updater

    bl ttc0_en_itvl         @ enable the counter in interval mode

    pop {lr}
    bx lr

@ resets and disables TTC0 counter 1
@ the counter must be reenabled manually
ttc0_reset:
	ldr r1, =(TTC0_BASE + TTC0_CNTCTL_OFF)
	mov r0, #cnt_dis_rst	@ assert reset
	str r0,[r1]		@ store constant
	bx lr

@ configures TTC0 to use prescale and pclk
@ sets div_value to 0 (div pclk by 2)
ttc0_clk_conf:
	ldr r1, =(TTC0_BASE+TTC0_CLKCTL_OFF)	@ load address of clock control reg
	mov r0, #1
	str r0, [r1]
	bx lr

@ sets the clock divider for
@ takes r0 as 4-bit prescale value
@ sets bits 4:1 in the TTC clock control register
ttc0_set_div:
	ldr r2, =TTC0_BASE
	ldr r1, [r2] 		@ get current conf
	ldr r3, =div_mask	@ load mask
	and r1, r3, r1 		@ mask out div bits
	and r0, r0, #0xF	@ mask out all but 4 LSB
	lsl r0,r0, #1		@ shift left one (align div bits)
	orr r1,r0, r1		@ combine r0,r1
	str r1, [r2]		@ write new conf
	bx lr

@ sets TTC0 counter 1 as enabled in interval mode
ttc0_en_itvl:
	ldr r1, =(TTC0_BASE + TTC0_CNTCTL_OFF)
	mov r0, #cnt_en_itvl	@ enable interval mode
	str r0, [r1]
	bx lr

@ takes in interval value (16-bit) as parameter
@ in r0, writes to itvl register
ttc0_set_itvl:
	ldr r1,=(TTC0_BASE+TTC0_ITVL_OFF)
	str r0, [r1]	@ store value
	bx lr

@ enables ttc0 interval interrupts
@ no parameter
ttc0_en_itvl_int:
	ldr r1, =(TTC0_BASE+TTC0_IER_OFF)
	mov r0, #intvl_int_en
	str r0, [r1]
	bx lr

@ sleep for approx 1ms
ttc0_poll: 
    ldr r1, =(TTC0_BASE + TTC0_ISR_OFF)
    b poll_loop

poll_loop:
    ldr r0, [r1]        @ read the ISR bit
    and r0, r0, #1      @ isolate LSB
    
    cmp r0, #0
    beq poll_loop       @ if the flag bit isn't asserted continue waiting

    bx lr               @ else exit