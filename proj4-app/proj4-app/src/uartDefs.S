@***************************
@  UART Register Addresses
@***************************

.equ UART1_BASE,     0xE0001000     @ Base register

.equ UART1_CR,       0xE0001000     @ Control Register
.equ UART1_MR,       0xE0001004     @ Mode Register
.equ UART1_SR,       0xE000102C     @ Status Register
.equ UART1_FIFO,     0xE0001030     @ TX/RX FIFO Data Port

.equ UART1_BAUDGEN,  0xE0001018     @ Sys Clk / BaudGen = RX Sampling Clk
.equ UART1_BAUDDIV,  0xE0001034     @ RX Sampling Clk / (BaudDiv + 1) = TX Clk


@***************************
@   UART Mode Constants
@***************************

.equ SET_MODE,   0x000
.equ STOP_BIT,   0x000
.equ PARITY,     0x020
.equ DATA_BITS,  0x000
.equ SYS_CLK,    0x000

.equ SETUP_OPCODE, SET_MODE | STOP_BIT | PARITY | DATA_BITS | SYS_CLK