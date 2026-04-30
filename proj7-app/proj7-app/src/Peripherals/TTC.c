#include "../Defs/TTC.h"

void configure_TTC0(uint32_t period_ticks, uint32_t pulse_ticks)
{
    /* Freeze counter before config */
    TTC0_CNT_CTRL(1) = TTC_CNT_DIS;

    /* Internal 111.111MHz clock, prescaler N=5 (divide by 64 -> ~0.576us/tick) */
    TTC0_CLK_CTRL(1) = TTC_CLK_PRESCALE(5) | TTC_CLK_EN;

    TTC0_INTERVAL(1) = (uint16_t)period_ticks;
    TTC0_MATCH(1, 1) = (uint16_t)pulse_ticks;

    // Enable interval interrupt
    TTC0_IER(1) = TTC_IER_IV;

    // Start: POL=1, WAVE_DIS=0 (waveform enabled, active low),
    // RST=1 (self-clearing), MATCH=1, INT=1, DIS=0 (running)
    // 0x40|0x10|0x08|0x02 = 0x5A
    TTC0_CNT_CTRL(1) = TTC_CNT_POL | TTC_CNT_RST | TTC_CNT_MATCH | TTC_CNT_INT;
}