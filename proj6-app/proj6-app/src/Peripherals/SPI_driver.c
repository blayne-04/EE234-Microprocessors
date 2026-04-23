#include "../Defs/SPI.h"

void reset_SPI(void)
{
    int i = 0;
    SLCR_UNLOCK = SLCR_UNLOCK_KEY;
    SLCR_SPI_RST = 0xF;
    for(i = 0; i < 1000; i++);
    SLCR_SPI_RST = 0;
    SLCR_LOCK = SLCR_LOCK_KEY;
}

void SPI_init(void)
{
    reset_SPI();
    SPI_ER = SPI_ER_DISABLE;
    SPI_CR = SPI_CR_INIT;
    SPI_ER = SPI_ER_ENABLE;
    SPI_IER = (1 << 4);
}

void SPI_write(uint32_t cs, uint8_t reg, uint8_t data)
{
    /* Select Slave */
    SPI_CR = (SPI_CR & ~SPI_CR_SS_MASK) | cs;

    /* Queue address & data in FIFO */
    SPI_TXD = LSM9DS1_WRITE_FLAG | reg; // Write to this address
    SPI_TXD = data; // Heres the actual data

    SPI_CR |= SPI_CR_START; // Begin transmitting

    /* Clear useless data slave sends back from RX */
    while (!SPI_SR_RX_HAS_DATA);
    (void)SPI_RXD;
    while (!SPI_SR_RX_HAS_DATA);
    (void)SPI_RXD;

    /* Deselect Slave */
    SPI_CR = (SPI_CR & ~SPI_CR_SS_MASK) | SPI_CR_SS_NONE;
}

uint8_t SPI_read(uint32_t cs, uint8_t reg)
{
    /* Select slave */
    SPI_CR = (SPI_CR & ~SPI_CR_SS_MASK) | cs;

    /* Queue address & data in FIFO */
    SPI_TXD = LSM9DS1_READ_FLAG | reg; // Wrie to this address
    SPI_TXD = 0x00; // Write nothing, (we're receiving)

    SPI_CR |= SPI_CR_START; // Begin transmitting

    /* Throw away the junk in the first bit, store the second bit */
    while (!SPI_SR_RX_HAS_DATA);
    (void)SPI_RXD;
    while (!SPI_SR_RX_HAS_DATA);
    uint8_t result = SPI_RXD;

    /* Deselect Slave */
    SPI_CR = (SPI_CR & ~SPI_CR_SS_MASK) | SPI_CR_SS_NONE;

    return result;
}


void SPI_start_gyro_read(void)
{
    SPI_CR = (SPI_CR & ~SPI_CR_SS_MASK) | SPI_CR_SS0;

    *(SPI0_BASE + 0x0B) = 7;

    SPI_TXD = LSM9DS1_READ_FLAG | AG_OUT_X_G;
    SPI_TXD = 0x00;
    SPI_TXD = 0x00;
    SPI_TXD = 0x00;
    SPI_TXD = 0x00;
    SPI_TXD = 0x00;
    SPI_TXD = 0x00;
    SPI_CR |= SPI_CR_START;
}