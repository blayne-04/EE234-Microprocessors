.text 
.global chal2

@define const
.set LED_CTL, 0x41210000
.set SW_DATA, 0x41220000
.set BTN_DATA, 0x41200000

chal2:
    MOV r4,#0
    @load button and sw ADDRESS into regs
    LDR r0,=BTN_DATA      @ r0 = button address
    LDR r1,=SW_DATA       @ r1 = switch address
    LDR r7,=LED_CTL       @ r7 = LED address

chal2Loop:
    @continuously update sw and btn DATA
    LDR r5,[r0]           @ r5 = button value
    LDR r6,[r1]           @ r6 = switch value

    @bitmask buttons we care about, btn 1 = latch, btn 2 = reset
    AND r2,r5,#0x01       @ latch button
    AND r3,r5,#0x02       @ reset button

    @if latch == 0, branch to reset
    CMP r2,#0
    beq reset

    @else latch == 1, latch switches
    AND r6,r6,#0xFF
    MOV r4,r6
    b ledUpdate

reset:
    @if reset button not asserted, do nothing
    CMP r3,#0
    BEQ ledUpdate
    
    @else clear latch
    MOV r4,#0
    b ledUpdate
    
ledUpdate:
    @store latched value into LEDs
    STR r4,[r7]
    b chal2Loop