#include "uart.h"

char rxBuffer[128];

int main(void)
{
    /* Configure UART */
    configure_uart1();

    /* Req 5. Call assembly funcs */
    uartReceiveStr(rxBuffer, 128);
    uartSendStr(rxBuffer);
    uart1_send_char('\n');

    /* Req 6. Call C Implementations*/
    while(1)
    {
        uart1_get_ln(rxBuffer, 128);
        uart1_send_str(rxBuffer);
        uart1_send_char('\n');
    }
}