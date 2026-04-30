#include "../Defs/TTC.h"

void configure_TTC0(uint32_t period_ticks, uint32_t pulse_ticks)
{
    // 1. Freeze counter before configuration (OD=1)
    TTC0_CNT_CTRL(1) = TTC_CNT_OD;

    // 2. Clock: internal 111MHz PS clock, prescaler N=5 -> divides by 2^6 = 64
    //    Resulting tick rate: 111.111MHz / 64 = 1,736,109 Hz (~0.576us/tick)
    //    CS=0 (internal clock), CE=0 (positive edge), EN=1 (prescale enabled)
    TTC0_CLK_CTRL(1) = TTC_CLK_PRESCALE(5) | TTC_CLK_EN;

    // 3. Set period: 34722 ticks * 0.576us = ~20ms (50Hz)
    TTC0_INTERVAL(1) = (uint16_t)period_ticks;

    // 4. Set pulse width: 2604 ticks * 0.576us = ~1.5ms (center)
    TTC0_MATCH(1, 1) = (uint16_t)pulse_ticks;

    // 5. Reset counter to start cleanly (CR is self-clearing)
    TTC0_CNT_CTRL(1) = TTC_CNT_CR;

    // 6. Start counter with full config:
    //    PL=1: active-high pulse (high at interval start, low at match)
    //    OW=1: output waveform enabled -> drives J1 pin
    //    ME=1: match enable
    //    IM=1: interval mode (resets at period_ticks)
    //    OD=0: counter running (NOT frozen)
    TTC0_CNT_CTRL(1) = TTC_CNT_PL | TTC_CNT_OW | TTC_CNT_ME | TTC_CNT_IM;

    // 7. Enable interval interrupt
    TTC0_IER(1) = TTC_IER_IV;
}
