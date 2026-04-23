#include "../Defs/Wrappers.h"

volatile int16_t req3_gyro_x = 0;
volatile int16_t req3_gyro_y = 0;
volatile int16_t req3_gyro_z = 0;

volatile int req3_spi_flag   = 0;

void req3_handler(void)
{
    uint32_t id = ICCIAR;

    switch(id) {
        case GTC_INT_ID:
            GTC_ISR = 0x01;
            SPI_start_gyro_read();
            break;

        case SPI_INT_ID: {
            (void)SPI_RXD;  // discard junk byte

            uint8_t xl = SPI_RXD, xh = SPI_RXD;
            uint8_t yl = SPI_RXD, yh = SPI_RXD;
            uint8_t zl = SPI_RXD, zh = SPI_RXD;

            SPI_CR = (SPI_CR & ~SPI_CR_SS_MASK) | SPI_CR_SS_NONE;

            req3_gyro_x = (int16_t)((xh << 8) | xl);
            req3_gyro_y = (int16_t)((yh << 8) | yl);
            req3_gyro_z = (int16_t)((zh << 8) | zl);

            req3_spi_flag = 1;
            SPI_SR = (1 << 4);
            break;
        }
    }

    ICCEOIR = id;
}

void req3(void)
{
    configure_uart1();
    SPI_init();
    enable_gyro();
    configure_gtc_1s();

    disable_ARM_interrupts();
    register_irq_handler(req3_handler);
    configure_GIC();
    enable_arm_interrupts();

    int counter = 0;
    while(1) {
        counter++;
        if (counter % 5000000 == 0) {
            uart1_send_str("main loop still running...\r\n");
        }

        if (req3_spi_flag) {
            req3_spi_flag = 0;
            char buf[64];

            sprintf(buf, "X: 0x%04X  Y: 0x%04X  Z: 0x%04X\r\n",
                    (uint16_t)req3_gyro_x,
                    (uint16_t)req3_gyro_y,
                    (uint16_t)req3_gyro_z);
            uart1_send_str(buf);
        }
    }
}