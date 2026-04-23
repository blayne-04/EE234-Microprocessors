#include "../Defs/I2C.h"

void reset_I2C(void)
{
    SLCR_UNLOCK = SLCR_UNLOCK_KEY;
    SLCR_I2C_RST = 0x3;
    SLCR_I2C_RST = 0;
    SLCR_LOCK = SLCR_LOCK_KEY;
}

void configure_i2c(void)
{
    reset_I2C();
    // DIVA=0, DIVB=12 → ~400KHz, master mode, 7-bit addr, ACK enabled
    I2C1_CR = (0 << 14) | (12 << 8) | (1 << 3) | (1 << 2) | (1 << 1);
    I2C1_CR |= (1 << 6);  // clear FIFO
    I2C1_IER = (1 << 0); // enable interrupts
}

void I2C_start_temp_read(void)
{
    I2C1_ISR = 0x1FF;
    I2C1_CR |= (1 << 0); //receive 
    I2C1_TRANS_SIZE = 2;
    I2C1_ADDR = LM75B_ADDR; // triggers transfer, returns immediately
}

void I2C_wait_complete(void)
{
    while (!(I2C1_ISR & (1 << 0)));
    I2C1_ISR = (1 << 0);
}