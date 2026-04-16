#include "../Defs/CPU_GIC.h"

/**
 * @brief Sends a null-terminated string over UART1.
 * * This function polls the UART Status Register (SR) to ensure the 
 * Transmit FIFO is not full before writing each character.
 * * @param msg Pointer to the string to be transmitted.
 */
void isr_putstr(char *msg) {
    // Loop until the null terminator ('\0') is reached
    while (*msg) {
        // Wait while the Transmit FIFO is FULL (Bit 4 of UART1_SR)
        // This prevents overwriting data in the 64-byte hardware buffer.
        while (UART1_SR & (1 << 4)); 

        // Write the current character to the Data FIFO and advance the pointer
        UART1_FIFO = *msg++;
    }
}

volatile int req3_counter = 0;
volatile int req3_divider = 0;

void subroutine_handler() {
    uint32_t id = ICCIAR;

    switch(id) {
        case UART1_INT_ID:
            while(!(UART1_SR & (1 << 1))) { // RX not empty
                char rx = (char)UART1_FIFO;
                if (!blink_active) {
                    UART1_FIFO = rx; // Req 1: Echo
                    if (rx >= '1' && rx <= '9') target_cycles = rx - '0'; // Req 5: Set count
                }
            }
            UART1_ISR = 0x01;
            break;

        case GTC_INT_ID:
            if (blink_active) {
                // ... (Keep existing Req 5 blink logic)
            } else {
                // REQUIREMENT 3: 1-second counter logic
                req3_divider++;
                if (req3_divider >= 2) {
                    req3_counter++;
                    // Print the last digit of the counter
                    while(UART1_SR & (1 << 4)); // Wait if full
                    UART1_FIFO = (req3_counter % 10) + '0';
                    req3_divider = 0;
                }
            }
            GTC_ISR = 0x01;
            break;

        case GPIO_INT_ID:
        {
            uint32_t status = GPIO_INT_STAT(1);
            // BTN4: Start (Req 5)
            if ((status & BTN4_MASK) && !blink_active && target_cycles > 0) {
                blink_active = 1;
                toggle_count = 0;
                isr_putstr("\r\nStarted\r\n");
                GTC_CR |= (1 << 2); // Unmute GTC IRQ
            }
            // BTN5: Stop/Reset (Req 5)
            if (status & BTN5_MASK) {
                blink_active = 0;
                target_cycles = 0;
                GTC_CR &= ~(1 << 2);
                GPIO_DATA(0) &= ~RGB_ALL_MASK;
                isr_putstr("\r\nStopped\r\n");
            }
            GPIO_INT_STAT(1) = status;
            break;
        }
    }
    ICCEOIR = id;
}

/**
 * FLAG MODE (Req 4)
 * Logic: Just set flags. Processing happens in main's while(1).
 */
void flag_handler() {
    uint32_t id = ICCIAR;
    switch(id) {
        case UART1_INT_ID:
            if (!(UART1_SR & (1 << 1))) {
                last_uart_char = (char)UART1_FIFO;
                uart_flag = 1;
            }
            UART1_ISR = 0x01;
            break;
        case GTC_INT_ID:
            gtc_flag = 1;
            GTC_ISR = 0x01;
            break;
        case GPIO_INT_ID:
            last_gpio_status = GPIO_INT_STAT(1);
            gpio_flag = 1;
            GPIO_INT_STAT(1) = last_gpio_status;
            break;
    }
    ICCEOIR = id;
}