#include "Wrappers.h"

#define MATCH_MIN   1736  // 1.0ms
#define MATCH_MAX   3472  // 2.0ms
#define SWEEP_STEP  7     // ~4us per update (7 * 0.576us)

/**
 * ISR for Requirement 2
 * Updates the match value every 20ms to create a smooth 5-second sweep.
 */
void irq_handler2(void)
{
    uint32_t int_id = ICCIAR;

    if (int_id == TTC0_1_INT_ID) {
        // Clear the TTC interrupt status bit
        TTC0_ISR(1);

        static uint32_t current_match = MATCH_MIN;
        static int direction = 1; // 1 for increasing (CCW), -1 for decreasing (CW)

        // Increment or decrement the pulse width
        if (direction == 1) {
            LED_DATA ^= 0x01;
            current_match += SWEEP_STEP;
            if (current_match >= MATCH_MAX) {
                current_match = MATCH_MAX;
                direction = -1; // Reverse at the 2.0ms limit
            }
        } else {
            LED_DATA ^= 0x01;
            current_match -= SWEEP_STEP;
            if (current_match <= MATCH_MIN) {
                current_match = MATCH_MIN;
                direction = 1; // Reverse at the 1.0ms limit
            }
        }

        // Apply the new position to the hardware
        TTC0_MATCH(1, 1) = (uint16_t)current_match;
    }

    ICCEOIR = int_id;
}

void req2(void)
{
    /* Same as usual */
    disable_ARM_interrupts();
    configure_uart1();
    configure_GIC();  
    
    /* Register the sweep handler */
    register_irq_handler(irq_handler2);

    /* Init TTC0 at min angle */
    configure_TTC0(34722, 30000);

    enable_arm_interrupts();
    
    uart1_send_str("Requirement 2: Smooth 5s Sweep Active.\r\n");
}