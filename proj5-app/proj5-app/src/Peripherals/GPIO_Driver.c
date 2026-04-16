#include "../Defs/GPIO.h"

void configure_gpio()
{
    /* --- RGB LED Configuration (Bank 0) --- */
    // Set Direction to Output (1) for RED LED
    GPIO_DIRM(0) |= RGB_ALL_MASK; 
    // Enable Output for RED LED
    GPIO_OEN(0)  |= RGB_ALL_MASK;

    /* --- Pushbutton Configuration (Bank 1) --- */
    // Set Direction to Input (0) for BTN4 and BTN5
    GPIO_DIRM(1) &= ~(BTN4_MASK | BTN5_MASK);

    /* --- GPIO Interrupt Configuration (Bank 1) --- */
    // 1. Set Interrupt Type to Edge Sensitive (1)
    GPIO_INT_TYPE(1) |= (BTN4_MASK | BTN5_MASK);
    
    // 2. Set Interrupt Polarity to Rising Edge (1)
    GPIO_INT_POL(1)  |= (BTN4_MASK | BTN5_MASK);
    
    // 3. Ensure "Any Edge" is disabled (0) to strictly follow Rising Edge
    GPIO_INT_ANY(1)  &= ~(BTN4_MASK | BTN5_MASK);

    // 4. Enable interrupts for these specific pins in the GPIO module
    GPIO_INT_EN(1)    = (BTN4_MASK | BTN5_MASK);
}