#ifndef BLACKBOARD_HW_H
#define BLACKBOARD_HW_H

#include <stdint.h>
// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// ============================================================
// SPI0 Register Addresses
// ============================================================
#define SPI0_BASE               ((volatile uint32_t *) 0xE0006000)
#define SPI_CR                  (*(SPI0_BASE + 0x00))
#define SPI_SR                  (*(SPI0_BASE + 0x01))
#define SPI_IER                 (*(SPI0_BASE + 0x02))  // 0x08 - Interrupt Enable Register
#define SPI_IDR                 (*(SPI0_BASE + 0x03))  // 0x0C - Interrupt Disable Register
#define SPI_IMR                 (*(SPI0_BASE + 0x04))  // 0x10 - Interrupt Mask Register
#define SPI_ER                  (*(SPI0_BASE + 0x05))
#define SPI_TXD                 (*(SPI0_BASE + 0x07))
#define SPI_RXD                 (*(SPI0_BASE + 0x08))

// ============================================================
// SLCR Addresses
// ============================================================
#define SLCR_LOCK               (*((volatile uint32_t *) 0xF8000004))
#define SLCR_UNLOCK             (*((volatile uint32_t *) 0xF8000008))
#define SLCR_SPI_RST            (*((volatile uint32_t *) 0xF800021C))
#define SLCR_UNLOCK_KEY         0xDF0D
#define SLCR_LOCK_KEY           0x767B

// ============================================================
// SPI_CR - Configuration Register write values
// OR these together and assign to SPI_CR
// ============================================================
#define SPI_CR_MASTER_MODE      (1 << 0)    // 1 = master mode
#define SPI_CR_CPOL_HIGH        (1 << 1)    // 1 = clock idle high (CPOL=1)
#define SPI_CR_CPHA_LATE        (1 << 2)    // 1 = sample on second edge (CPHA=1)
#define SPI_CR_BAUD_DIV_32      (4 << 3)    // ~5.2MHz (166MHz / 2^5)
#define SPI_CR_NO_EXT_DECODE    (0 << 7)    // 0 = drive SS lines directly
#define SPI_CR_SS_NONE          (0xF << 10) // 1111 = no slave selected
#define SPI_CR_SS0              (0xE << 10) // 1110 = accel/gyro selected
#define SPI_CR_SS1              (0xD << 10) // 1101 = magnetometer selected
#define SPI_CR_SS_MASK          (0xF << 10) // mask to clear SS bits before setting
#define SPI_CR_MANUAL_CS        (1 << 14)   // 1 = software controls chip select
#define SPI_CR_MANUAL_START_EN  (1 << 15)   // 1 = transfer only starts on command
#define SPI_CR_START            (1 << 16)   // 1 = issue start command
#define SPI_CR_INIT  (SPI_CR_MASTER_MODE | SPI_CR_CPOL_HIGH | SPI_CR_CPHA_LATE | SPI_CR_BAUD_DIV_32 | SPI_CR_NO_EXT_DECODE | SPI_CR_SS_NONE | SPI_CR_MANUAL_CS | SPI_CR_MANUAL_START_EN)

// ============================================================
// SPI_SR - Status Register reads
// These include the register so they can be used directly as conditions
// ============================================================
#define SPI_SR_RX_HAS_DATA      (SPI_SR & (1 << 4))  // nonzero = RX FIFO has data
#define SPI_SR_TX_HAS_SPACE     (SPI_SR & (1 << 2))  // nonzero = TX FIFO not full

// ============================================================
// SPI_ER - Enable Register write values
// ============================================================
#define SPI_ER_ENABLE           (1 << 0)    // 1 = enable SPI controller
#define SPI_ER_DISABLE          (0 << 0)    // 0 = disable SPI controller


// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

// ============================================================
// LSM9DS1 SPI transaction flags
// OR with register address to form first byte of transaction
// ============================================================
#define LSM9DS1_READ_FLAG       (1 << 7)    // first byte bit 7 = 1 for read
#define LSM9DS1_WRITE_FLAG      (0 << 7)    // first byte bit 7 = 0 for write

// ============================================================
// LSM9DS1 Accel/Gyro Registers (SS0)
// ============================================================
#define AG_WHO_AM_I             0x0F    // should return 0x68
#define AG_CTRL_REG1_G          0x10    // gyro data rate, scale, bandwidth
#define AG_CTRL_REG6_XL         0x20    // accel data rate, scale, bandwidth
#define AG_OUT_X_G              0x18    // gyro X axis (16-bit, 2 regs)
#define AG_OUT_Y_G              0x1A    // gyro Y axis (16-bit, 2 regs)
#define AG_OUT_Z_G              0x1C    // gyro Z axis (16-bit, 2 regs)
#define AG_OUT_X_XL             0x28    // accel X axis (16-bit, 2 regs)
#define AG_OUT_Y_XL             0x2A    // accel Y axis (16-bit, 2 regs)
#define AG_OUT_Z_XL             0x2C    // accel Z axis (16-bit, 2 regs)

// ============================================================
// LSM9DS1 Magnetometer Registers (SS1)
// ============================================================
#define MAG_WHO_AM_I            0x0F    // should return 0x3D
#define MAG_CTRL_REG3           0x22    // operating mode control
#define MAG_OUT_X               0x28    // mag X axis (16-bit, 2 regs)
#define MAG_OUT_Y               0x2A    // mag Y axis (16-bit, 2 regs)
#define MAG_OUT_Z               0x2C    // mag Z axis (16-bit, 2 regs)

// ============================================================
// LSM9DS1 Initialisation values
// ============================================================
#define AG_CTRL_REG1_G_INIT     0xA0    // gyro on, 476Hz ODR
#define AG_CTRL_REG6_XL_INIT    0xA0    // accel on, 476Hz ODR
#define MAG_CTRL_REG3_INIT      0x00    // continuous conversion mode


// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


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


/* --- Global Timer (GTC) Registers (Base: 0xF8F00200) --- */
#define GTC_DR_LOW  (*((volatile uint32_t *) 0xF8F00200)) // Counter Low 32-bits
#define GTC_DR_HIGH (*((volatile uint32_t *) 0xF8F00204)) // Counter High 32-bits
#define GTC_CR      (*((volatile uint32_t *) 0xF8F00208)) // Control Register
#define GTC_ISR     (*((volatile uint32_t *) 0xF8F0020C)) // Interrupt Status
#define GTC_COMP_L  (*((volatile uint32_t *) 0xF8F00210)) // Compare Low
#define GTC_COMP_H  (*((volatile uint32_t *) 0xF8F00214)) // Compare High
#define GTC_AI      (*((volatile uint32_t *) 0xF8F00218)) // Auto-Increment


// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


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


// ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


// I2C1 Registers (Base: 0xE0005000)
#define I2C1_CR         (*((volatile uint32_t *) 0xE0005000))
#define I2C1_SR         (*((volatile uint32_t *) 0xE0005004))
#define I2C1_ADDR       (*((volatile uint32_t *) 0xE0005008))
#define I2C1_DATA       (*((volatile uint32_t *) 0xE000500C))
#define I2C1_ISR        (*((volatile uint32_t *) 0xE0005010))
#define I2C1_TRANS_SIZE (*((volatile uint32_t *) 0xE0005014))
#define I2C1_IMR        (*((volatile uint32_t *) 0xE0005020))
#define I2C1_IER        (*((volatile uint32_t *) 0xE0005024))
#define I2C1_IDR        (*((volatile uint32_t *) 0xE0005028))

#define LM75B_ADDR      0x48  // 0b1001000
#define SLCR_I2C_RST    (*((volatile uint32_t *) 0xF8000224))
#endif