#include "../Defs/Inertial.h"

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
    uint8_t low  = acc_gyro_read(address_low);
    uint8_t high = acc_gyro_read(address_low + 1);
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