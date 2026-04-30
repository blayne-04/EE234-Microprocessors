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
// TTC0 Register Addresses (Base: 0xF8001000)
// Offsets per counter n (1,2,3): base_offset + (n-1)*4
// ============================================================
#define TTC0_BASE               0xF8001000

// TTC0_CLK_CTRL(n): 0x00/04/08
#define TTC0_CLK_CTRL(n)        (*((volatile uint32_t *)(TTC0_BASE + 0x00 + ((n)-1)*4)))

// TTC0_CNT_CTRL(n): 0x0C/10/14
#define TTC0_CNT_CTRL(n)        (*((volatile uint32_t *)(TTC0_BASE + 0x0C + ((n)-1)*4)))

// TTC0_CNTVAL(n): 0x18/1C/20  (read only)
#define TTC0_CNTVAL(n)          (*((volatile uint32_t *)(TTC0_BASE + 0x18 + ((n)-1)*4)))

// TTC0_INTERVAL(n): 0x24/28/2C
#define TTC0_INTERVAL(n)        (*((volatile uint32_t *)(TTC0_BASE + 0x24 + ((n)-1)*4)))

// TTC0_MATCH(n, m): 0x30-0x50, 9 registers total (3 per counter)
// Counter n (1-3), Match register m (1-3)
#define TTC0_MATCH(n, m)        (*((volatile uint32_t *)(TTC0_BASE + 0x30 + ((n)-1)*12 + ((m)-1)*4)))

// TTC0_ISR(n): 0x54/58/5C  (clear on read)
#define TTC0_ISR(n)             (*((volatile uint32_t *)(TTC0_BASE + 0x54 + ((n)-1)*4)))

// TTC0_IER(n): 0x60/64/68
#define TTC0_IER(n)             (*((volatile uint32_t *)(TTC0_BASE + 0x60 + ((n)-1)*4)))

// TTC0_EVENTCNTL(n): 0x6C/70/74
#define TTC0_EVENTCNTL(n)       (*((volatile uint32_t *)(TTC0_BASE + 0x6C + ((n)-1)*4)))

// TTC0_EVENT(n): 0x78/7C/80  (read only)
#define TTC0_EVENT(n)           (*((volatile uint32_t *)(TTC0_BASE + 0x78 + ((n)-1)*4)))

// ============================================================
// TTC0 CLK_CTRL bit definitions  (TTC0_CLKCNT_X)
// ============================================================
#define TTC_CLK_CE              (1 << 6)    // Negative edge select (external clk only)
#define TTC_CLK_CS              (1 << 5)    // Clock source: 1=external, 0=111MHz PS
#define TTC_CLK_PRESCALE(n)     (((n) & 0xF) << 1) // Prescale value N; divides by 2^(N+1)
#define TTC_CLK_EN              (1 << 0)    // Prescale enable

// ============================================================
// TTC0 CNT_CTRL bit definitions  (TTC0_CNTL_X)
// ============================================================
#define TTC_CNT_PL              (1 << 6)    // Polarity: 1=high-to-low on match (active high pulse)
#define TTC_CNT_OW              (1 << 5)    // Output waveform enable
#define TTC_CNT_CR              (1 << 4)    // Counter reset (self-clearing)
#define TTC_CNT_ME              (1 << 3)    // Match enable
#define TTC_CNT_DC              (1 << 2)    // Decrement (down count)
#define TTC_CNT_IM              (1 << 1)    // Interval mode
#define TTC_CNT_OD              (1 << 0)    // Output disable/freeze (1=freeze, 0=run)


// ============================================================
// TTC0 IER bit definitions  (TTC0_IER_X)
// ============================================================
#define TTC_IER_EV              (1 << 5)    // Event timer overflow interrupt enable
#define TTC_IER_CO              (1 << 4)    // Counter overflow interrupt enable
#define TTC_IER_M3              (1 << 3)    // Match 3 interrupt enable
#define TTC_IER_M2              (1 << 2)    // Match 2 interrupt enable
#define TTC_IER_M1              (1 << 1)    // Match 1 interrupt enable
#define TTC_IER_IV              (1 << 0)    // Interval interrupt enable

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