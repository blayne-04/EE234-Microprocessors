#include "Defs/UART.h"
#include "Defs/CPU_GIC.h"
#include "Defs/GPIO.h"
#include "Defs/GTC.h"
#include "Defs/Handlers.h"

int main(void) {
    // TOGGLE: 0 = Req 5 (Subroutine), 1 = Req 4 (Flag)
    run_mode = 0; 

    configure_uart1();
    configure_gpio();

    disable_ARM_interrupts();
    
    // Switch registration
    if (run_mode == 1) register_irq_handler(flag_handler);
    else register_irq_handler(subroutine_handler);

    configure_GIC();
    configure_uart1_interrupt();
    enable_arm_interrupts();
    configure_gtc(run_mode); 

    // Req 4 variables
    int tick_divider = 0;
    int seconds_count = 0;

    while(1) {
        if (run_mode == 1) {
            // Process Flags
            if (uart_flag) {
                UART1_FIFO = last_uart_char;
                uart_flag = 0;
            }
            if (gtc_flag) {
                tick_divider++;
                if (tick_divider >= 2) {
                    seconds_count++;
                    UART1_FIFO = (seconds_count % 10) + '0';
                    tick_divider = 0;
                }
                gtc_flag = 0;
            }
            if (gpio_flag) {
                if (last_gpio_status & BTN4_MASK) GPIO_DATA(0) |= RGB_ALL_MASK;
                if (last_gpio_status & BTN5_MASK) GPIO_DATA(0) &= ~RGB_ALL_MASK;
                gpio_flag = 0;
            }
        }
    }
}