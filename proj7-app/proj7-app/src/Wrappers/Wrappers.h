#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <stdio.h>
#include <stdlib.h>
#include "../Defs/Blackboard_HW.h"
#include "../Defs/CPU_GIC.h"
#include "../Defs/UART.h"
#include "../Defs/TTC.h"
#include "../Defs/XADC.h"

void req1(void);
void irq_handler1(void);

void req2(void);
void irq_handler2(void);

void req3(void);
void irq_handler3(void);

void req4(void);
void irq_handler4(void);

void chal1(void);
void irq_handler5(void);

#endif