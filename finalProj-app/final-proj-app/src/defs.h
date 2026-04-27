#ifndef HARDWARE_DEFS_H
#define HARDWARE_DEFS_H

#include <stdint.h>

/* GPIO Defs */
#define LED_DATA    (*(volatile uint32_t *)0x41210000)
#define SW_DATA     (*(volatile uint32_t *)0x41220000)
#define BTN_DATA    (*(volatile uint32_t *)0x41200000)

/* RGB LED EN & PWM Init */
#define RED_LED     (*(volatile uint32_t *)0x43C00000)
#define BLUE_LED    (*(volatile uint32_t *)0x43C00020)
#define RED_PERIOD  (*(volatile uint32_t *)0x43C00004)
#define RED_WIDTH   (*(volatile uint32_t *)0x43C00008)
#define BLUE_PERIOD (*(volatile uint32_t *)0x43C00024)
#define BLUE_WIDTH  (*(volatile uint32_t *)0x43C00028)

/* ADC REGISTERS */
#define XADC_BASE   0x43C50000
#define XADC_CTL    (*(volatile uint32_t *)(XADC_BASE + 0x300))
#define XADC_DATA   (*(volatile uint32_t *)(XADC_BASE + 0x20C))

#define CH_VP       3

uint32_t read_adc(void);
void xadc_select_potentiometer(void);
void red_blue_led_driver(uint32_t adc_val);

#endif