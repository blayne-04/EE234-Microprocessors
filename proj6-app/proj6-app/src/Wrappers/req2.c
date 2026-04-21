#include "../Defs/Wrappers.h"

void req2(void)
{
    configure_uart1();
    SPI_init();
    enable_accel();

    char buf[64];
    while(1)
    {
        int16_t x = (int16_t)read_axis(AG_OUT_X_XL);
        int16_t y = (int16_t)read_axis(AG_OUT_Y_XL);
        int16_t z = (int16_t)read_axis(AG_OUT_Z_XL);

        int32_t x_mg = ((int32_t)x * 61) / 1000;
        int32_t y_mg = ((int32_t)y * 61) / 1000;
        int32_t z_mg = ((int32_t)z * 61) / 1000;

        uart1_send_str("--- Accelerometer ---\r\n");

        sprintf(buf, "X raw: 0x%04X  Roll:    %ld mg\r\n", (uint16_t)x, x_mg);
        uart1_send_str(buf);

        sprintf(buf, "Y raw: 0x%04X  Pitch:   %ld mg\r\n", (uint16_t)y, y_mg);
        uart1_send_str(buf);

        sprintf(buf, "Z raw: 0x%04X  Gravity: %ld mg\r\n", (uint16_t)z, z_mg);
        uart1_send_str(buf);

        uart1_send_str("\r\n");

        for(volatile int i = 0; i < 50000000; i++);
    }
}