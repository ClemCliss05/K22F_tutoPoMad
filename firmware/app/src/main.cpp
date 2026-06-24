#include "clock.hpp"
#include "gpio.hpp"

int main() {
    Bsp::Clock clock;
    clock.initOSC();
    clock.set48MHz();

    Drivers::Gpio::LED_Init();
    Drivers::Gpio::PBs_Init();
    Drivers::Gpio::LED_On();
    for (volatile uint32_t i = 0; i < 5000000; i++)
        ;
    Drivers::Gpio::LED_Off();

    while (1) {
        if (Drivers::Gpio::PB1_GetState()) {
            Drivers::Gpio::LED_On(Drivers::Gpio::LED_GREEN);
        } else {
            Drivers::Gpio::LED_Off(Drivers::Gpio::LED_GREEN);
        }

        if (Drivers::Gpio::PB2_GetState()) {
            Drivers::Gpio::LED_On(Drivers::Gpio::LED_BLUE);
        } else {
            Drivers::Gpio::LED_Off(Drivers::Gpio::LED_BLUE);
        }
    }
}