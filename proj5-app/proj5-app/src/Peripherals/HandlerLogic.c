#include "../Defs/CPU_GIC.h"

void isr_putstr(char *msg) {
    // Loop until the null terminator ('\0') is reached
    while (*msg) {
        // Wait while the Transmit FIFO is FULL (Bit 4 of UART1_SR)
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
            while (!(UART1_SR & (1 << 1))) { // While RX not empty
                char rx = (char)UART1_FIFO;
                if (blink_active) break;      // Discard if blinking
                if (rx >= '1' && rx <= '9') {
                    target_cycles = rx - '0';
                    UART1_FIFO = rx;           // Echo back confirmation
                }
            }
            UART1_ISR = 0x01;
            break;

        case GTC_INT_ID:
            GTC_ISR = 0x01; // Clear flag
            if (!blink_active) break;

            toggle_count++;

            // Toggle LED every half second
            if (GPIO_DATA(0) & RGB_ALL_MASK)
                GPIO_DATA(0) &= ~RGB_ALL_MASK;
            else
                GPIO_DATA(0) |= RGB_ALL_MASK;

            // Each full cycle = 2 toggles
            if (toggle_count >= target_cycles * 2) {
                blink_active  = 0;
                toggle_count  = 0;
                target_cycles = 0;
                GPIO_DATA(0) &= ~RGB_ALL_MASK; // Ensure LED off
                GTC_CR &= ~(0x0E);             // Disable IE, CE, AI
                isr_putstr("\r\nDone\r\n");
            }
            break;

        case GPIO_INT_ID: {
            uint32_t status = GPIO_INT_STAT(1);
            GPIO_INT_STAT(1) = status; // Clear flags immediately

            if (status & BTN4_MASK) {
                if (!blink_active && target_cycles > 0) {
                    blink_active = 1;
                    toggle_count = 0;
                    // Sync compare to current time before enabling
                    GTC_COMP_L = GTC_DR_LOW + 166666667;
                    GTC_COMP_H = GTC_DR_HIGH;
                    GTC_ISR    = 0x01;
                    GTC_CR    |= 0x0E; // Enable IE, CE, AI
                    isr_putstr("\r\nStarted\r\n");
                }
            }

            if (status & BTN5_MASK) {
                blink_active  = 0;
                toggle_count  = 0;
                target_cycles = 0;
                GTC_CR       &= ~(0x0E); // Disable IE, CE, AI
                GPIO_DATA(0) &= ~RGB_ALL_MASK;
                isr_putstr("\r\nStopped\r\n");
            }
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