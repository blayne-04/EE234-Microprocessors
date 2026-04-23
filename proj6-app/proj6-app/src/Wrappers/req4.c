#include "../Defs/Wrappers.h"

void req4(void)
{
    configure_uart1();
    configure_i2c();

    char buf[32];
    uint8_t last_btn = 0;

    while(1) {
        uint8_t btn = (GPIO_DATA_RO(1) & BTN4_MASK) ? 1 : 0;

        if (btn && !last_btn) {
            I2C_start_temp_read();
            I2C_wait_complete();
            float temp = parse_temperature();

            int whole = (int)temp;
            int frac  = (int)((temp - whole) * 1000);
            sprintf(buf, "Temp: %d.%03d C\r\n", whole, frac);
            uart1_send_str(buf);
        }
        last_btn = btn;
    }
}