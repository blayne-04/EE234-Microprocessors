#ifndef I2C_H
#define I2C_H

#include "Blackboard_HW.h"

void reset_I2C(void);
void configure_i2c(void);
void I2C_wait_complete(void);

void I2C_start_temp_read(void);


#endif