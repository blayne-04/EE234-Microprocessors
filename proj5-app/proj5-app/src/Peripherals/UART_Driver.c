#include "../Defs/UART.h"

void configure_uart1()
{
	UART1_CR = 0x00000003;
	while((UART1_CR & 0x00000003) != 0);

	UART1_MR = SETUP_OPCODE;

	UART1_BAUD = 0x7C;
	UART1_BDIV = 6;

    /** * Bit 0: RX Enable (1)
     * Bit 2: TX Enable (1)
     * Bit 4: TX Reset  (1)
     */
    UART1_CR = (1 << 4) | (1 << 2) | (1 << 0); // 0x15
}

void configure_uart1_interrupt()
{
	// Set RX FIFO Trigger Level
	UART1_RXWM = 1;

	// Interrupt Enable
	UART1_IER = 1;
}