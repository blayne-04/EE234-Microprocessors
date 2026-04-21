#ifndef INERTIAL_H
#define INERTIAL_H

#include "Blackboard_HW.h"
#include "SPI.h"

uint32_t acc_gyro_read(uint32_t address);
uint32_t mag_read(uint32_t address);

void write_acc_gyro(uint32_t address, uint32_t data);
uint16_t read_axis(uint32_t address_low);
void enable_accel(void);
void enable_gyro(void);


#endif
