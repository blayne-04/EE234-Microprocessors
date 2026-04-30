#include "Wrappers.h"


void irq_handler1(void)
{
    /* Check ICCIAR */
    uint32_t int_id = ICCIAR;

    /* Redundant since its the only configured but may as well.. future insurance */
    if (int_id == TTC0_1_INT_ID) {
        LED_DATA ^= 0x01;
        /* Clear status */
        TTC0_ISR(1);
    }

    /* Hey GIC, we're done here.*/
    ICCEOIR = int_id;
}

void req1(void)
{
    /* Init Hardware */
    disable_ARM_interrupts();
    configure_uart1();
    configure_GIC();

    /* Apply irq_handler1 to vector table */
    register_irq_handler(irq_handler1);

    // 3. Configure TTC0 for a 20ms period and 1.5ms center pulse
    // Ticks calculated for 111.111MHz clock with a 64 prescaler
    configure_TTC0(34722, 2604);

    /* Enable interrupts! */
    enable_arm_interrupts();

    uart1_send_str("Requirement 1: Servo Centered.\r\n");
}