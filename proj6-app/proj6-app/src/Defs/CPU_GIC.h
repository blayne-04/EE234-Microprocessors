#ifndef CPU_GIC_H
#define CPU_GIC_H

#include "Blackboard_HW.h"
#include <xil_exception.h>

/* --- Flags --- */
extern volatile int uart_flag;
extern volatile int gtc_flag;
extern volatile int gpio_flag;
extern volatile char last_uart_char;
extern volatile uint32_t last_gpio_status;
extern volatile int spi_flag;
extern volatile int i2c_flag;

/* --- CPU Interface --- */
void enable_arm_interrupts();
void disable_ARM_interrupts();
void register_irq_handler(void (*handler_ptr)(void));

/* --- GIC Configuration --- */
void configure_GIC();
void configure_GIC_ID(uint32_t ID, uint32_t priority, uint32_t sensitivity);

/* --- Interrupt Flag Handler --- */
void flag_handler();

#endif