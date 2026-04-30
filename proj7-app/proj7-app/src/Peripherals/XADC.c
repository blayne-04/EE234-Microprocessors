#include "../Defs/XADC.h"

uint32_t read_adc()
{
    return (XADC_DATA >> 4) & 0xFFF;
}

/* Configure ADC to read potentiometer */
void xadc_select_potentiometer()
{
    uint32_t reg_temp;
    /* Mask a temp register with the CTL to modify */    
    reg_temp = XADC_CTL;
    /* & to clear bits */
    reg_temp &= 0xFFE0;
    /* OR with VP (Potentiometer channel & limit to 5 bits)*/
    reg_temp |= (3 & 0x1F); /* 3 = CH_VP */
    /* Apply to CTL reg */
    XADC_CTL = reg_temp;
}