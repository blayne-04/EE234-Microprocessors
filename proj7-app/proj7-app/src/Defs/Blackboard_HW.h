#ifndef BLACKBOARD_HW_H
#define BLACKBOARD_HW_H

#include <stdint.h>

// ============================================================
// UART1 Register Addresses
// ============================================================
#define UART1_CR        (*((volatile unsigned int *) 0xE0001000))
#define UART1_MR        (*((volatile unsigned int *) 0xE0001004))
#define UART1_SR        (*((volatile unsigned int *) 0xE000102C))
#define UART1_FIFO      (*((volatile unsigned int *) 0xE0001030))
#define UART1_BAUDGEN   (*((volatile unsigned int *) 0xE0001018))
#define UART1_BAUDDIV   (*((volatile unsigned int *) 0xE0001034))

// ============================================================
// UART1 Mode Register config
// ============================================================
#define SET_MODE        (0x0 << 8)  // normal mode
#define STOP_BIT        (0x0 << 6)  // 1 stop bit
#define PARITY          (0x4 << 3)  // no parity
#define DATA_BITS       (0x0 << 1)  // 8 data bits
#define SYS_CLK         (0x0 << 0)  // reference clock
#define SETUP_OPCODE    (SET_MODE | STOP_BIT | PARITY | DATA_BITS | SYS_CLK)

// ============================================================
// Baud Rate config (115200)
// ============================================================
#define BAUDGEN_115200  0x7C
#define BAUDDIV_115200  6

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

/* --- Interrupt IDs --- */
#define GTC_INT_ID      27
#define GPIO_INT_ID     52
#define SPI_INT_ID      58
#define I2C_INT_ID      80
#define TTC0_1_INT_ID   42


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


// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// ============================================================
// TTC0 Registers (Base: 0xF8001000, counter n = 1/2/3)
// ============================================================
#define TTC0_BASE            0xF8001000

#define TTC0_CLK_CTRL(n)     (*((volatile uint32_t *)(TTC0_BASE + 0x00 + ((n)-1)*4)))
#define TTC0_CNT_CTRL(n)     (*((volatile uint32_t *)(TTC0_BASE + 0x0C + ((n)-1)*4)))
#define TTC0_INTERVAL(n)     (*((volatile uint32_t *)(TTC0_BASE + 0x24 + ((n)-1)*4)))
#define TTC0_MATCH(n, m)     (*((volatile uint32_t *)(TTC0_BASE + 0x30 + ((n)-1)*12 + ((m)-1)*4)))
#define TTC0_ISR(n)          (*((volatile uint32_t *)(TTC0_BASE + 0x54 + ((n)-1)*4)))
#define TTC0_IER(n)          (*((volatile uint32_t *)(TTC0_BASE + 0x60 + ((n)-1)*4)))

// CNT_CTRL bits (verified against Zynq-7000 TRM)
#define TTC_CNT_POL          (1 << 6)  // Polarity: 1 = active-high pulse
#define TTC_CNT_WAVE_DIS     (1 << 5)  // Waveform disable (ACTIVE LOW: 0 = enabled)
#define TTC_CNT_RST          (1 << 4)  // Counter reset (self-clearing)
#define TTC_CNT_MATCH        (1 << 3)  // Match enable
#define TTC_CNT_INT          (1 << 1)  // Interval mode
#define TTC_CNT_DIS          (1 << 0)  // Counter disable (1 = stopped)

// CLK_CTRL bits
#define TTC_CLK_PRESCALE(n)  (((n) & 0xF) << 1)  // Divides clock by 2^(N+1)
#define TTC_CLK_EN           (1 << 0)             // Prescale enable

// IER bits
#define TTC_IER_IV           (1 << 0)  // Interval interrupt enable

// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// ============================================================
// XADC Registers
// ============================================================
#define XADC_BASE           0x43C50000
#define XADC_CTL            (*((volatile uint32_t *)(XADC_BASE + 0x300)))
#define XADC_DATA           (*((volatile uint32_t *)(XADC_BASE + 0x20C)))

// ============================================================
// LED for debugging ISR
// ============================================================
#define LED_DATA    (*(volatile uint32_t *)0x41210000)

#endif