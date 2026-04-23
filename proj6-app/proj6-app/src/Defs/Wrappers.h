#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdio.h>
#include "UART.h"
#include "Blackboard_HW.h"
#include "CPU_GIC.h"
#include "GTC.h"
#include "GPIO.h"
#include "Peripheral.h"
#include "SPI.h"
#include "I2C.h"

void req1(void);
void req2(void);
void req3(void);
void req3_handler(void);
void req4(void);
void req5(void);
void req5_handler(void);

#endif