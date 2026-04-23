#ifndef SPI_I2C_H
#define SPI_I2C_H

#include "Blackboard_HW.h"

void reset_SPI(void);
void config_SPI(void);

void SPI_init(void);

void SPI_write(uint32_t cs, uint8_t reg, uint8_t data);
uint8_t SPI_read(uint32_t cs, uint8_t reg);

void SPI_start_gyro_read(void);


#endif