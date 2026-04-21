#include "../Defs/Wrappers.h"

void req3(void)
{
    configure_uart1();
    SPI_init();
    enable_accel();
    configure_GIC();
    configure_gtc_1s();
    register_irq_handler(flag_handler);
    enable_arm_interrupts();

    char buf[64];
    while(1)
    {
        if(gtc_flag)
        {
            gtc_flag = 0;
            spi_flag = 0;  // clear any stale SPI flag

            int16_t x = (int16_t)read_axis(AG_OUT_X_XL);
            int16_t y = (int16_t)read_axis(AG_OUT_Y_XL);
            int16_t z = (int16_t)read_axis(AG_OUT_Z_XL);

            int32_t x_mg = ((int32_t)x * 61) / 1000;
            int32_t y_mg = ((int32_t)y * 61) / 1000;
            int32_t z_mg = ((int32_t)z * 61) / 1000;

            uart1_send_str("--- Accelerometer ---\r\n");
            sprintf(buf, "X Roll:    %ld mg\r\n", x_mg);
            uart1_send_str(buf);
            sprintf(buf, "Y Pitch:   %ld mg\r\n", y_mg);
            uart1_send_str(buf);
            sprintf(buf, "Z Gravity: %ld mg\r\n", z_mg);
            uart1_send_str(buf);
            uart1_send_str("\r\n");
        }
    }
}