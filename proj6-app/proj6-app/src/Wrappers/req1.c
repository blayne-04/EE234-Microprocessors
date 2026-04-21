#include "../Defs/Wrappers.h"

void req1(void)
{
    configure_uart1();
    SPI_init();

    while(1)
    {
        uart1_send_str("BOOT\r\n");

        /* should return 0x68 */
        uint32_t ag_id  = acc_gyro_read(AG_WHO_AM_I);
        /* should return 0x3D */
        uint32_t mag_id = mag_read(MAG_WHO_AM_I);

        char buf[64];
        sprintf(buf, "Accel/Gyro WHO_AM_I: 0x%02X\r\n", ag_id);
        uart1_send_str(buf);

        sprintf(buf, "Magnetometer WHO_AM_I: 0x%02X\r\n", mag_id);
        uart1_send_str(buf);

        for(volatile int i = 0; i < 50000000; i++);
    }
}