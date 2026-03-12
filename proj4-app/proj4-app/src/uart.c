#include "uart.h"

/****************************
 *      UART Functions
 ****************************/

/***
* FUNC: Configure UART
* 1. Reset
* 2. Load config macro from header into mode reg for 115200 baud, 8 data bits, no parity, 1 stop bit
* 3. Setup Baudgen and divider
* 4. Enable TX & RX Channels
***/
void configure_uart1(void)
{
    resetUart1();

    *(volatile unsigned int *)UART1_MR = SETUP_OPCODE;

    *(volatile unsigned int *)UART1_BAUDGEN = 0x7C;
    *(volatile unsigned int *)UART1_BAUDDIV = 6;

    *(volatile unsigned int *)UART1_CR = (4 | 16);
}

/***
* FUNC: Reset UART
* 1. Load control register
* 2. Assert reset
* 3. Wait until reset is successfull
***/
void resetUart1(void)
{
    volatile unsigned int *cr = (volatile unsigned int *)UART1_CR;
    *cr = 3;

    while((*cr & 3) != 0);
}

/***
* FUNC: Get Char Over UART
* 1. Load status register
* 2. Check status reg until RX FIFO has data
* 3. Return char
***/
char uart1_getchar(void)
{
    volatile unsigned int *sr = (volatile unsigned int *)UART1_SR;

    while((*sr & 2) != 0);

    return *(volatile unsigned char *)UART1_FIFO;
}

/***
* FUNC: Send Char Over UART
* 1. Load status register
* 2. Check status reg until TX FIFO has space
* 3. Store char @ UART FIFO addr
***/
void uart1_send_char(char data)
{
    volatile unsigned int *sr = (volatile unsigned int *)UART1_SR;

    while((*sr & 16) != 0);

    *(volatile unsigned char *)UART1_FIFO = data;
}

/***
* FUNC: Send Str Over UART
* 1. While buffer* != '\0'
* 2. Call send char, post inc by 1
***/
void uart1_send_str(char buffer[])
{
    while(*buffer)
    {
        uart1_send_char(*buffer++);
    }
}

/***
* FUNC: Get Line
* 1. Init iterator = 0, temp char c
* 2. While i < bufferSize - 1
* 3. Check for '\n' and '\r', the 2 delimiters for windows, if found break
* 4. Copy tempChar into buffer
* 5. Return num of chars copied
***/
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