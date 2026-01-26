.text
.global req3

@define const
.set LED_CTL, 0x41210000
.set BTN_DATA, 0x41200000

@init buttons and leds into registers
req3:
    ldr r1,=BTN_DATA
    ldr r2,=LED_CTL

@control loop
req3_loop:
    ldr r0,[r1]
    str r0,[r2]
    b req3_loop