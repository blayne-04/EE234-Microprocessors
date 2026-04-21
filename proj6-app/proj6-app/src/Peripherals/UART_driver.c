#include "../Defs/UART.h"

void resetUart1(void)
{
    UART1_CR = 3;
    while((UART1_CR & 3) != 0);
}

void configure_uart1(void)
{
    resetUart1();
    UART1_MR      = SETUP_OPCODE;
    UART1_BAUDGEN = BAUDGEN_115200;
    UART1_BAUDDIV = BAUDDIV_115200;
    UART1_CR      = (4 | 16);
}

char uart1_getchar(void)
{
    while((UART1_SR & 2) != 0);
    return (char) UART1_FIFO;
}

void uart1_send_char(char data)
{
    while((UART1_SR & 16) != 0);
    UART1_FIFO = data;
}

void uart1_send_str(char buffer[])
{
    while(*buffer)
    {
        uart1_send_char(*buffer++);
    }
}

int uart1_get_ln(char buffer[], int max)
{
    int i = 0;
    char c;
    while(i < (max - 1))
    {
        c = uart1_getchar();
        if(c == '\n' || c == '\r')
        {
            break;
        }
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0';
    return i;
}