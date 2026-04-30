#include "Wrappers.h"

#define MATCH_MIN   1736  // 1.0ms
#define MATCH_MAX   3472  // 2.0ms
#define MATCH_RANGE 1736  // (3472 - 1736)

volatile uint32_t req4_target_match = 2604;
volatile int req4_speed_step = 7;

/* move servo toward "target match" at a smooth pace */
void irq_handler4(void)
{
    uint32_t int_id = ICCIAR;

    if (int_id == TTC0_1_INT_ID) {
        TTC0_ISR(1);

        uint32_t actual = TTC0_MATCH(1, 1);
        LED_DATA ^= 0x01;
        // Move toward target at the selected speed
        if (actual < req4_target_match) {
            actual += req4_speed_step;
            if (actual > req4_target_match) actual = req4_target_match;
        } 
        else if (actual > req4_target_match) {
            if (actual > (req4_target_match + req4_speed_step)) {
                actual -= req4_speed_step;
            } else {
                actual = req4_target_match;
            }
        }

        TTC0_MATCH(1, 1) = (uint16_t)actual;
    }

    ICCEOIR = int_id;
}


void req4(void)
{
    disable_ARM_interrupts();
    configure_uart1();       
    configure_GIC();         
    
    register_irq_handler(irq_handler4);
    configure_TTC0(34722, 2604);       

    enable_arm_interrupts();
    
    uart1_send_str("Requirement 4 Ready: \r\n");
    uart1_send_str("Commands: [p] Position (0-100), [v] Velocity (1-5)\r\n");

    while(1) {
        char cmd = uart1_getchar();

        if (cmd == 'p') {
            uart1_send_str("Enter Position (0-100): ");
            char buf[4];
            uart1_get_ln(buf, 4);
            int pos = atoi(buf);
            
            if (pos < 0) pos = 0;
            if (pos > 100) pos = 100;

            // Map 0-100 to 1736-3472
            req4_target_match = MATCH_MIN + ((pos * MATCH_RANGE) / 100);
        }
        else if (cmd == 'v') {
            uart1_send_str("Enter Speed (1=Slow, 5=Fast): ");
            char c = uart1_getchar();
            // Scale step size: 1 -> 3 ticks, 5 -> 35 ticks
            req4_speed_step = (c - '0') * 7; 
        }
    }
}