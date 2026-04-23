#include "../Defs/Peripheral.h"

uint32_t acc_gyro_read(uint32_t address) {
    return SPI_read(SPI_CR_SS0, address);
}

uint32_t mag_read(uint32_t address) {
    return SPI_read(SPI_CR_SS1, address);
}

void write_acc_gyro(uint32_t address, uint32_t data)
{
    SPI_write(SPI_CR_SS0, address, data);
}

uint16_t read_axis(uint32_t address_low)
{
    SPI_CR = (SPI_CR & ~SPI_CR_SS_MASK) | SPI_CR_SS0;
    SPI_TXD = LSM9DS1_READ_FLAG | address_low;
    SPI_TXD = 0x00;
    SPI_TXD = 0x00;
    SPI_CR |= SPI_CR_START;

    while (!SPI_SR_RX_HAS_DATA); (void)SPI_RXD;  // discard junk
    while (!SPI_SR_RX_HAS_DATA); uint8_t low  = SPI_RXD;
    while (!SPI_SR_RX_HAS_DATA); uint8_t high = SPI_RXD;

    SPI_CR = (SPI_CR & ~SPI_CR_SS_MASK) | SPI_CR_SS_NONE;
    return (uint16_t)((high << 8) | low);
}

void enable_accel(void)
{
    write_acc_gyro(AG_CTRL_REG6_XL, AG_CTRL_REG6_XL_INIT);
}

void enable_gyro(void)
{
    write_acc_gyro(AG_CTRL_REG1_G, AG_CTRL_REG1_G_INIT);
}

float parse_temperature(void)
{
    uint8_t msb = I2C1_DATA & 0xFF;
    uint8_t lsb = I2C1_DATA & 0xFF;
    int16_t raw = (int16_t)((msb << 8) | lsb) >> 5;
    return raw * 0.125f;
}