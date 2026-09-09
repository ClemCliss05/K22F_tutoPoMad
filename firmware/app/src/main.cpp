#include "clock.hpp"
#include "gpio.hpp"
#include "pit.hpp"

#include "uart.hpp"
#include "uart_logger_backend.hpp"
#include "logger.hpp"
#include "ringbuffer.hpp"

#include "MK22FN512.h"

void delay(uint32_t amount) {
    for (volatile uint32_t i = 0; i < amount; i++)
    {
        __NOP();
    }
}

int main() {

    // Init Clock
    Bsp::Clock clock;
    clock.initOSC();
    clock.set48MHz();

    // Init GPIO
    Drivers::Gpio gpio;
    gpio.LED_Init();
    gpio.PBs_Init();
    using LedColor = Drivers::Gpio::LedColor;

    // Init UART
    Drivers::Uart uart;
    uart.init();
    UartLoggerBackend uartBackend(uart);
    char loggerBuffer[128];
    RingBuffer ringBuffer(loggerBuffer, sizeof(loggerBuffer));
    Logger logger(ringBuffer, uartBackend);
    
    LOG_DEBUG("Boot");
    LOG_DEBUG("Clock OK");
    LOG_DEBUG("UART OK");

    // Initialize PIT channel[0]
    Drivers::Pit pit;
	pit.init();
	LOG_DEBUG("PIT OK");

    gpio.LED_On();
    pit.delayMs(1000);
    gpio.LED_Off();

    while (1) {
        gpio.LED_On(LedColor::Green);
        pit.delayMs(1000);
        gpio.LED_Off();
        gpio.LED_On(LedColor::Blue);
        pit.delayMs(1000);
        gpio.LED_Off();
    }
}