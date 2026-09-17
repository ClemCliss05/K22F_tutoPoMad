#include "clock.hpp"
#include "delay.hpp"
#include "gpio.hpp"
#include "pit.hpp"

#include "logger.hpp"
#include "ringbuffer.hpp"
#include "uart.hpp"
#include "uart_logger_backend.hpp"

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
    using LedColor = Drivers::Gpio::LedColor;

    // Init UART
    Drivers::Uart uart;
    uart.init();
    Services::UartLoggerBackend uartBackend(uart);
    char loggerBuffer[128];
    RingBuffer ringBuffer(loggerBuffer, sizeof(loggerBuffer));
    Logger logger(ringBuffer, uartBackend);
    LOG_DEBUG("UART OK");

    // Initialize PIT channel[0]
    Drivers::Pit pit(clock.getBusClock());
    pit.init();
    LOG_DEBUG("PIT OK");
    Services::Delay delay(pit);
    LOG_DEBUG("DELAY OK");

    gpio.LED_On();
    delay.ms(1000);
    gpio.LED_Off();

    while (1) {
        gpio.LED_On(LedColor::Green);
        delay.ms(1000);
        gpio.LED_Off();
        gpio.LED_On(LedColor::Blue);
        delay.ms(1000);
        gpio.LED_Off();
    }
}