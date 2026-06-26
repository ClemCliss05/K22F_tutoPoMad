#include "clock.hpp"
#include "gpio.hpp"
#include "uart.hpp"
#include "MK22FN512.h"

int main() {

    // Init Clock
    Bsp::Clock clock;
    clock.initOSC();
    clock.set48MHz();

    // Init GPIO
    Drivers::Gpio gpio;
    gpio.LED_Init();
    gpio.PBs_Init();

    // Init UART
    Drivers::Uart uart;
    uart.init();
    uint8_t	sent;

    gpio.LED_On();
    for (volatile uint32_t i = 0; i < 5000000; i++)
    {
        __NOP();
    }
    gpio.LED_Off();

    while (1) {
        if (gpio.PB1_GetState()) {
            gpio.LED_On(Drivers::Gpio::LedColor::Green);
            // Send 'G' only once
			if (sent == 0)
			{
				while ((UART1->S1 & UART_S1_TC_MASK) != UART_S1_TC_MASK);
				UART1->D = 'G';
				sent = 1;
			}
        } else {
            gpio.LED_Off(Drivers::Gpio::LedColor::Green);
            sent = 0;
        }

        if (gpio.PB2_GetState()) {
            gpio.LED_On(Drivers::Gpio::LedColor::Blue);
            // Send 'B' only once
            if(sent == 0)
            {
                while ((UART1->S1 & UART_S1_TC_MASK) != UART_S1_TC_MASK);
                UART1->D = 'B';
                sent = 1;
            }   
        } else {
            gpio.LED_Off(Drivers::Gpio::LedColor::Blue);
            sent = 0;
        }
    }
}