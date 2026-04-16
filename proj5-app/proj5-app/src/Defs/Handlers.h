#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdint.h>
#include "../Defs/Blackboard_HW.h"

/* --- System Mode --- */
/** 0 = Subroutine Mode (Req 5: Smart ISR), 1 = Flag Mode (Req 4: Dumb ISR) */
extern volatile int run_mode; 

/* --- Requirement 5 State Variables (Blink Sequence) --- */
extern volatile int blink_active;      /** 0 = Idle, 1 = Blinking */
extern volatile int target_cycles;     /** User-specified blinks (1-9) */
extern volatile int toggle_count;      /** Half-second state changes */

/* --- Requirement 4 Flags (Latency Optimization) --- */
extern volatile int uart_flag;
extern volatile int gtc_flag;
extern volatile int gpio_flag;
extern volatile char last_uart_char;
extern volatile uint32_t last_gpio_status;

/* --- Logic Handlers --- */

/** * Requirement 1, 2, 3, 5 Handler
 * Performs all logic, counting, and UART echos inside the interrupt context.
 */
void subroutine_handler();

/** * Requirement 4 Handler
 * Only captures data and sets flags. All processing is deferred to main.
 */
void flag_handler();

/** * Utility: Sends a string over UART safely during an interrupt.
 * Polls the Transmit Full bit to prevent FIFO overflow.
 */
void isr_putstr(char *msg);

#endif