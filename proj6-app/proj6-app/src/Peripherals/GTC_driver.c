#include "../Defs/GTC.h"

void configure_gtc_1s(void)
{
    GTC_CR = 0;                     // disable during config

    GTC_DR_LOW  = 0;                // reset counter
    GTC_DR_HIGH = 0;

    GTC_COMP_L  = 333333333;        // 1 second at 333MHz
    GTC_COMP_H  = 0;

    GTC_AI      = 333333333;        // auto-increment 1 second

    GTC_ISR     = 0x01;             // clear any pending interrupt

    GTC_CR      = 0x0F;             // enable counter, comparator, interrupt, auto-increment
}