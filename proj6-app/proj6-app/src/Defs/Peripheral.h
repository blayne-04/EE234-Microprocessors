#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "Blackboard_HW.h"
#include "SPI.h"

uint32_t acc_gyro_read(uint32_t address);
uint32_t mag_read(uint32_t address);

void write_acc_gyro(uint32_t address, uint32_t data);
void enable_accel(void);
void enable_gyro(void);
uint16_t read_axis(uint32_t address_low);

float parse_temperature(void);

#endif
