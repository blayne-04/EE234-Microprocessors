#ifndef CPU_GIC_H
#define CPU_GIC_H

#include "Blackboard_HW.h"
#include <xil_exception.h>

/* --- CPU Interface --- */
void enable_arm_interrupts();
void disable_ARM_interrupts();
void register_irq_handler(void (*handler_ptr)(void));

/* --- GIC Configuration --- */
void configure_GIC();
void configure_GIC_ID(uint32_t ID, uint32_t priority, uint32_t sensitivity);

#endif