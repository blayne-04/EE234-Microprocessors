.text 
.global chal3

@define const
.set LED_CTL, 0x41210000
.set BTN_DATA, 0x41200000

@-------------------------------------------------------------------
@IMPORTANT NOTE: Execution continues downward even if we dont branch
@-------------------------------------------------------------------
chal3:
    @load addresses
    LDR r0,=BTN_DATA    @button address
    LDR r1,=LED_CTL     @led address
    MOV r3,#0           @r3 will be a previous state tracker  
    MOV r4,#0           @r4 will be the led state tracker

chal3_loop:
    @Read in button status, mask only 1 button
    LDR r2,[r0]
    AND r2,r2,#0x01

    @ if(r2 == 1 && r3 == 0)
    CMP r2,#1
    BNE keepItPushing
    CMP r3,#0
    BNE keepItPushing

    @ invert led state with an xor
    EOR r4,r4,#0x01
    @ don't need this but I prefer to write it explicitly
    b keepItPushing 

keepItPushing:
    @store current btn state to previous btn state
    MOV r3,r2
    STR r4,[r1]

    b chal3_loop