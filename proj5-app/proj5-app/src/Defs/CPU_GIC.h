#ifndef CPU_GIC_H
#define CPU_GIC_H

#include "../Defs/Blackboard_HW.h"
#include "xil_exception.h"

/* --- Shared State Variables --- */
extern volatile int blink_active;      /** 0 = Idle, 1 = Blinking (Req 5) */
extern volatile int target_cycles;     /** Number of blinks 1-9 (Req 5) */
extern volatile int toggle_count;      /** Half-second counter (Req 5) */

extern volatile int run_mode;          /** 0 = Subroutine Mode, 1 = Flag Mode */

/* --- Requirement 4 Flags --- */
extern volatile int uart_flag;
extern volatile int gtc_flag;
extern volatile int gpio_flag;
extern volatile char last_uart_char;
extern volatile uint32_t last_gpio_status;

/* --- CPU Interface --- */
void enable_arm_interrupts();
void disable_ARM_interrupts();
void register_irq_handler(void (*handler_ptr)(void));

/* --- GIC Configuration --- */
void configure_GIC();
void configure_GIC_ID(uint32_t ID, uint32_t priority, uint32_t sensitivity);

#endif