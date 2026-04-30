#include "Wrappers.h"
#include "Wrappers.h"

#define MATCH_MIN    1736   // 1.0ms
#define MATCH_CENTER 2604   // 1.5ms
#define MATCH_MAX    3472   // 2.0ms
#define MANUAL_STEP  100    // Visible incremental step

// Current position shared between Main (UART) and ISR
volatile uint32_t manual_match = MATCH_CENTER;

/* Make sure the hardware register is in sync with our software */
void irq_handler3(void)
{
    uint32_t int_id = ICCIAR;

    if (int_id == TTC0_1_INT_ID) {
        TTC0_ISR(1); /* Clear Status */

        /* Update the hardware with the current manually selected position */
        TTC0_MATCH(1, 1) = (uint16_t)manual_match;
    }

    ICCEOIR = int_id;
}

void req3(void)
{
    disable_ARM_interrupts();
    configure_uart1();
    configure_GIC();  
    
    /* Register manual control handler */
    register_irq_handler(irq_handler3);

    /* Init TTC0 centered */
    configure_TTC0(34722, MATCH_CENTER);

    enable_arm_interrupts();
    
    uart1_send_str("Requirement 3: Terminal Control Active.\r\n");
    uart1_send_str("Use 'w' for CCW, 's' for CW.\r\n");

    while(1) {
        /* Poll for char */
        char cmd = uart1_getchar();

        if (cmd == 'w') { // CCW (Increase pulse width)
            manual_match += MANUAL_STEP;
            if (manual_match > MATCH_MAX) manual_match = MATCH_MAX;
        } 
        else if (cmd == 's') { // CW (Decrease pulse width)
            if (manual_match >= (MATCH_MIN + MANUAL_STEP)) {
                manual_match -= MANUAL_STEP;
            } else {
                manual_match = MATCH_MIN;
            }
        }
    }
}