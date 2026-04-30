#include "Wrappers.h"

#include "Wrappers.h"

#define MATCH_MIN   1736  // 1.0ms
#define MATCH_RANGE 1736  // (3472 - 1736)

void irq_handler5(void)
{
    uint32_t int_id = ICCIAR;

    if (int_id == TTC0_1_INT_ID) {
        TTC0_ISR(1); // Clear TTC status

        // 1. Get the current potentiometer value (0-4095)
        uint32_t adc_val = read_adc();

        // 2. Map ADC to Servo Ticks: Min + (ADC * Range / 4095)
        uint32_t match_val = MATCH_MIN + ((adc_val * MATCH_RANGE) / 4095);

        // 3. Update hardware
        TTC0_MATCH(1, 1) = (uint16_t)match_val;
    }

    ICCEOIR = int_id;
}

void chal1(void)
{
    disable_ARM_interrupts();
    configure_uart1();
    configure_GIC();  
    xadc_select_potentiometer();
    register_irq_handler(irq_handler5);

    /* TTC @ 50 HZ */
    configure_TTC0(34722, 2604);

    enable_arm_interrupts();
    
    uart1_send_str("Challenge: Potentiometer Control Active.\r\n");
}