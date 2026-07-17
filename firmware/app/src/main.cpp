#include "clock.hpp"
#include "gpio.hpp"

#include "uart.hpp"
#include "uart_logger_backend.hpp"
#include "logger.hpp"
#include "ringbuffer.hpp"

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
    UartLoggerBackend uartBackend(uart);
    char loggerBuffer[128];
    RingBuffer ringBuffer(loggerBuffer, sizeof(loggerBuffer));
    Logger logger(ringBuffer, uartBackend);

    uint8_t greenSent = 0;
    uint8_t blueSent = 0;
    uint32_t i = 0;
    
    LOG_DEBUG("Boot");
    LOG_DEBUG("Clock OK");
    LOG_DEBUG("UART OK");

    gpio.LED_On();
    for (volatile uint32_t i = 0; i < 5000000; i++)
    {
        __NOP();
    }
    gpio.LED_Off();

    while (1) {
        if (gpio.PB1_GetState()) {
            gpio.LED_On(Drivers::Gpio::LedColor::Green);
            // Send 'Green' only once
			if (greenSent == 0)
			{
				LOG_INFO("Green %d", i);
				greenSent = 1;
                i++;
			}
        } else {
            gpio.LED_Off(Drivers::Gpio::LedColor::Green);
            greenSent = 0;
        }

        if (gpio.PB2_GetState()) {
            gpio.LED_On(Drivers::Gpio::LedColor::Blue);
            // Send 'Blue' only once
            if(blueSent == 0)
            {
                LOG_INFO("Blue %d", i);
                blueSent = 1;
                i++;
            }   
        } else {
            gpio.LED_Off(Drivers::Gpio::LedColor::Blue);
            blueSent = 0;
        }
    }
}