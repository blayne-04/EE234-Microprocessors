#include "../Defs/GTC.h"

void configure_gtc1()
{
    // 1. Disable the timer and comparator while configuring
    GTC_CR = 0;

    // 2. Reset the 64-bit counter to 0
    GTC_DR_LOW = 0;
    GTC_DR_HIGH = 0;

    // 3. Set the comparator for 0.5 seconds (166,500,000 = 0x09ECA500)
    GTC_COMP_L = 0x09ECA500;
    GTC_COMP_H = 0;

    // 4. Set the Auto-Increment value to 0.5 seconds
    GTC_AI = 0x09ECA500;

    // 5. Clear any pending interrupt status
    GTC_ISR = 0x01;

    GTC_CR = 0x01;
}

void configure_gtc2() {
    GTC_CR = 0;                          // Disable everything
    GTC_COMP_L = GTC_DR_LOW + 166666667; // ~0.5s at 333MHz
    GTC_COMP_H = GTC_DR_HIGH;
    GTC_AI     = 166666667;              // Auto-increment 0.5s
    GTC_ISR    = 0x01;                   // Clear stale flag
    GTC_CR = 0x0F;
}

void configure_gtc(volatile int run_mode)
{
    if(run_mode == 0){
        configure_gtc1();
    }
    else {
        configure_gtc2();
    }
}