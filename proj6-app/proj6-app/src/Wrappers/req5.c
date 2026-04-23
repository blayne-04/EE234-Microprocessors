#include "../Defs/Wrappers.h"

volatile float req5_temp  = 0.0f;
volatile int req5_i2c_flag = 0;

void req5_handler(void)
{
    uint32_t id = ICCIAR;

    switch(id) {
        case GTC_INT_ID:
            GTC_ISR = 0x01;
            I2C_start_temp_read();
            break;

        case I2C_INT_ID:
            req5_temp = parse_temperature();
            req5_i2c_flag = 1;
            I2C1_ISR = (1 << 0);
            break;
    }

    ICCEOIR = id;
}

void req5(void)
{
    configure_uart1();
    configure_i2c();
    configure_gtc_1s();

    disable_ARM_interrupts();
    register_irq_handler(req5_handler);
    configure_GIC();
    enable_arm_interrupts();

    char buf[32];
    int counter = 0;
    
    while(1) {
        counter++;
        if (counter % 5000000 == 0) {
            uart1_send_str("main loop still running...\r\n");
        }

        if (req5_i2c_flag) {
            req5_i2c_flag = 0;
            int whole = (int)req5_temp;
            int frac  = (int)((req5_temp - whole) * 1000);
            sprintf(buf, "Temp: %d.%03d C\r\n", whole, frac);
            uart1_send_str(buf);
        }
    }
}