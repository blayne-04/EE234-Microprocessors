#include "../Defs/CPU_GIC.h"

/* --- CPU Interface Functions --- */

void enable_arm_interrupts() {
    uint32_t cpsr_val = 0;
    // Read the Current Program Status Register
    asm("mrs %0, cpsr\n" : "=r" (cpsr_val)); 

    cpsr_val &= ~(0xFF); // Clear lower 8 bits (mode and interrupt bits)
    cpsr_val |= 0x5F;    // Set bits to enable IRQ (Bit 7 = 0)

    // Write back to CPSR
    asm("msr cpsr, %0\n" : : "r" (cpsr_val)); 
}

void disable_ARM_interrupts() {
    uint32_t cpsr_val = 0;
    // Read the Current Program Status Register
    asm("mrs %0, cpsr\n" : "=r" (cpsr_val)); 

    cpsr_val &= ~(0xFF); // Clear lower 8 bits
    cpsr_val |= 0xDF;    // Set bits to disable IRQ (Bit 7 = 1)

    // Write back to CPSR
    asm("msr cpsr, %0\n" : : "r" (cpsr_val)); 
}

void register_irq_handler(void (*handler_ptr)(void)) {
    Xil_ExceptionRegisterHandler(5, (Xil_ExceptionHandler)handler_ptr, NULL);
}

/* GIC Config */
void configure_GIC() {
    ICDDCR = 0;      // Disable distributor for config
    ICCPMR = 0xFF;   // Allow all priorities
    ICCICR = 0x03;   // Enable CPU interface (Secure + Non-secure)

    configure_GIC_ID(TTC0_1_INT_ID, 128, 0x01); /* Level sensitive, prio 128*/

    ICDDCR = 0x01;   // Re-enable distributor
}

void configure_GIC_ID(uint32_t ID, uint32_t priority, uint32_t sensitivity) {
    ICDICER(ID / 32) = (1 << (ID % 32)); // Disable during config

    // Set Priority (8 bits per ID)
    uint32_t prio_shift = (ID % 4) * 8;
    ICDIPR(ID / 4) &= ~(0xFF << prio_shift);
    ICDIPR(ID / 4) |= ((priority & 0xF8) << prio_shift);

    // Set Target (8 bits per ID, target CPU0 = 0x01)
    uint32_t target_shift = (ID % 4) * 8;
    ICDIPTR(ID / 4) &= ~(0xFF << target_shift);
    ICDIPTR(ID / 4) |= (0x01 << target_shift);

    // Set Sensitivity (2 bits per ID)
    uint32_t sens_shift = (ID % 16) * 2;
    ICDICFR(ID / 16) &= ~(0x03 << sens_shift);
    ICDICFR(ID / 16) |= ((sensitivity & 0x03) << sens_shift);

    ICDISER(ID / 32) = (1 << (ID % 32)); // Enable
}