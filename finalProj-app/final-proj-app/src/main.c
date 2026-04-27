#include "defs.h"

uint32_t switch_val = 0;
uint32_t button_val = 0;

int main(void)
{
    xadc_select_potentiometer();

    /* Initialize PWM val (50% Brightness)*/
    RED_PERIOD = 1024;
    RED_WIDTH = 512;
    BLUE_PERIOD = 1024;
    BLUE_WIDTH = 512;

    for(;;)
    {
        button_val = BTN_DATA;

        if(button_val & 0x01)
        {
            switch_val = SW_DATA & 0xFFF;
            LED_DATA = switch_val;
        }
        else if(button_val & 0x02)
        {
            uint32_t adc_val = read_adc();
            LED_DATA = adc_val;

            red_blue_led_driver(adc_val);
        }
    }
}

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
    reg_temp |= (CH_VP & 0x1F);
    /* Apply to CTL reg */
    XADC_CTL = reg_temp;
}

void red_blue_led_driver(uint32_t adc_val)
{
    if (adc_val < switch_val)
    {
        RED_LED = 1;
        BLUE_LED = 0;
    }
    else if (adc_val > switch_val)
    {
        RED_LED = 0;
        BLUE_LED = 1;
    }
}