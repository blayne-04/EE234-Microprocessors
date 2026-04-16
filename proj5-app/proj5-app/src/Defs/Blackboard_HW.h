#ifndef BLACKBOARD_HW_H
#define BLACKBOARD_HW_H

#include <stdint.h>

/* --- Interrupt IDs --- */
#define GTC_INT_ID      27
#define GPIO_INT_ID     52
#define UART1_INT_ID    82

/* --- Generic Interrupt Controller (GIC) Registers --- */
// CPU Interface Registers
#define ICCICR      (*((volatile uint32_t *) 0xF8F00100)) // CPU Interface Control
#define ICCPMR      (*((volatile uint32_t *) 0xF8F00104)) // Interrupt Priority Mask
#define ICCIAR      (*((volatile uint32_t *) 0xF8F0010C)) // Interrupt Acknowledge
#define ICCEOIR     (*((volatile uint32_t *) 0xF8F00110)) // End of Interrupt

// Distributor Registers
#define ICDDCR      (*((volatile uint32_t *) 0xF8F01000)) // Distributor Control
#define ICDISER(n)  (*(((volatile uint32_t *) 0xF8F01100) + (n))) // Set-Enable
#define ICDICER(n)  (*(((volatile uint32_t *) 0xF8F01180) + (n))) // Clear-Enable
#define ICDIPR(n)   (*(((volatile uint32_t *) 0xF8F01400) + (n))) // Priority Registers
#define ICDIPTR(n)  (*(((volatile uint32_t *) 0xF8F01800) + (n))) // Processor Targets
#define ICDICFR(n)  (*(((volatile uint32_t *) 0xF8F01C00) + (n))) // Configuration (Sensitivity)

/* --- UART1 Registers (Base: 0xE0001000) --- */
#define UART1_CR    (*((volatile uint32_t *) 0xE0001000)) // Control Register
#define UART1_MR    (*((volatile uint32_t *) 0xE0001004)) // Mode Register
#define UART1_IER   (*((volatile uint32_t *) 0xE0001008)) // Interrupt Enable
#define UART1_IDR   (*((volatile uint32_t *) 0xE000100C)) // Interrupt Disable
#define UART1_IMR   (*((volatile uint32_t *) 0xE0001010)) // Interrupt Mask
#define UART1_ISR   (*((volatile uint32_t *) 0xE0001014)) // Interrupt Status
#define UART1_BAUD  (*((volatile uint32_t *) 0xE0001018)) // Baud Rate Generator
#define UART1_RXWM  (*((volatile uint32_t *) 0xE0001020)) // RX FIFO Trigger Level
#define UART1_SR    (*((volatile uint32_t *) 0xE000102C)) // Channel Status
#define UART1_FIFO  (*((volatile uint32_t *) 0xE0001030)) // Data FIFO
#define UART1_BDIV  (*((volatile uint32_t *) 0xE0001034)) // Baud Rate Divider

/* --- UART Config --- */
#define SET_MODE        0x000   // 00 = normal, 01 = auto echo, 10 = local loopback, 11 = remote loopback
#define STOP_BIT        0x000   // 00 = 1 stop bit, 01 = 1.5 stop bits, 10 = 2 stop bits
#define PARITY          0x020   // 000 = even, 001 = odd, 010 = space, 011 = mark, 100 = none
#define DATA_BITS       0x000   // 00 = 8 bits, 01 = 7 bits, 10 = 6 bits
#define SYS_CLK         0x000   // 00 = UART reference clock

#define SETUP_OPCODE (SET_MODE | STOP_BIT | PARITY | DATA_BITS | SYS_CLK)

/* --- Global Timer (GTC) Registers (Base: 0xF8F00200) --- */
#define GTC_DR_LOW  (*((volatile uint32_t *) 0xF8F00200)) // Counter Low 32-bits
#define GTC_DR_HIGH (*((volatile uint32_t *) 0xF8F00204)) // Counter High 32-bits
#define GTC_CR      (*((volatile uint32_t *) 0xF8F00208)) // Control Register
#define GTC_ISR     (*((volatile uint32_t *) 0xF8F0020C)) // Interrupt Status
#define GTC_COMP_L  (*((volatile uint32_t *) 0xF8F00210)) // Compare Low
#define GTC_COMP_H  (*((volatile uint32_t *) 0xF8F00214)) // Compare High
#define GTC_AI      (*((volatile uint32_t *) 0xF8F00218)) // Auto-Increment

/* --- PS GPIO (MIO) Registers (Base: 0xE000A000) --- */
// Bank 0 = MIO 0-31; Bank 1 = MIO 32-53
#define GPIO_DATA(n)    (*(((volatile uint32_t *) 0xE000A040) + (n))) // Output Data
#define GPIO_DATA_RO(n) (*(((volatile uint32_t *) 0xE000A060) + (n))) // Input Data (Read Only)
#define GPIO_DIRM(n)    (*(((volatile uint32_t *) 0xE000A204) + 16*(n))) // Direction
#define GPIO_OEN(n)     (*(((volatile uint32_t *) 0xE000A208) + 16*(n))) // Output Enable

// GPIO Interrupt Control (per bank)
#define GPIO_INT_MASK(n) (*(((volatile uint32_t *) 0xE000A20C) + 16*(n)))
#define GPIO_INT_EN(n)   (*(((volatile uint32_t *) 0xE000A210) + 16*(n)))
#define GPIO_INT_DIS(n)  (*(((volatile uint32_t *) 0xE000A214) + 16*(n)))
#define GPIO_INT_STAT(n) (*(((volatile uint32_t *) 0xE000A218) + 16*(n)))
#define GPIO_INT_TYPE(n) (*(((volatile uint32_t *) 0xE000A21C) + 16*(n)))
#define GPIO_INT_POL(n)  (*(((volatile uint32_t *) 0xE000A220) + 16*(n)))
#define GPIO_INT_ANY(n)  (*(((volatile uint32_t *) 0xE000A224) + 16*(n)))

/* --- MIO Pin Masks (Blackboard Specific) --- */
#define BTN4_MASK       0x00040000 // MIO 50 (Bank 1 bit 18)
#define BTN5_MASK       0x00080000 // MIO 51 (Bank 1 bit 19)
#define RGB_BLUE_MASK   0x00010000 // MIO 16 (Bank 0 bit 16)
#define RGB_RED_MASK    0x00020000 // MIO 17 (Bank 0 bit 17)
#define RGB_GREEN_MASK  0x00040000 // MIO 18 (Bank 0 bit 18)
#define RGB_ALL_MASK    0x00070000 // All RGB bits in Bank 0

#endif