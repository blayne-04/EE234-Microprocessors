#ifndef UART_H
#define UART_H

/***************************
 *  UART Register Addresses
 ***************************/

#define UART1_BASE      0xE0001000     // Base register

#define UART1_CR        0xE0001000     // Control Register
#define UART1_MR        0xE0001004     // Mode Register
#define UART1_SR        0xE000102C     // Status Register
#define UART1_FIFO      0xE0001030     // TX/RX FIFO Data Port

#define UART1_BAUDGEN   0xE0001018     // Sys Clk / BaudGen = RX Sampling Clk
#define UART1_BAUDDIV   0xE0001034     // RX Sampling Clk / (BaudDiv + 1) = TX Clk


/***************************
 *   UART Mode Constants
 ***************************/

#define SET_MODE        0x000   // 00 = normal, 01 = auto echo, 10 = local loopback, 11 = remote loopback
#define STOP_BIT        0x000   // 00 = 1 stop bit, 01 = 1.5 stop bits, 10 = 2 stop bits
#define PARITY          0x020   // 000 = even, 001 = odd, 010 = space, 011 = mark, 100 = none
#define DATA_BITS       0x000   // 00 = 8 bits, 01 = 7 bits, 10 = 6 bits
#define SYS_CLK         0x000   // 00 = UART reference clock

#define SETUP_OPCODE (SET_MODE | STOP_BIT | PARITY | DATA_BITS | SYS_CLK)


/****************************
 *      Other Constants
 ****************************/

#define RX_BUFFER_SIZE 128

/****************************
 *   Extern UART Functions
 ****************************/

 // Receives a string over UART on mpu, echos back to serial port on pc
extern void uartReceiveStr(char buffer[], int max);

// Sends a string to serial port on pc from mpu over UART
extern void uartSendStr(char buffer[]);

/****************************
 *      UART Functions
 ****************************/

// Configures UART1 for 115200 baud, 8-data bits, no parity, 1 stop bit
void configure_uart1(void);

// Reset UART config
void resetUart1(void);


// Return next char received over UART
char uart1_getchar(void);

// Send a char over UART
void uart1_send_char(char data);

// Send a null-terminated str over UART
void uart1_send_str(char buffer[]);

// Receives characters until newline or max buffer size reached
int uart1_get_ln(char buffer[], int max);

#endif