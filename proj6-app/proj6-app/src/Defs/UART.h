// UART.h
#ifndef UART_H
#define UART_H

#include "Blackboard_HW.h"

// ============================================================
// UART1 Functions
// ============================================================
void configure_uart1(void);
void resetUart1(void);
char uart1_getchar(void);
void uart1_send_char(char data);
void uart1_send_str(char buffer[]);
int  uart1_get_ln(char buffer[], int max);

#endif