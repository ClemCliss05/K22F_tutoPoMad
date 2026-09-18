#include "MK22FN512.h"

#include "clock.hpp"

#include "gpio.hpp"
#include "pit.hpp"
#include "uart.hpp"

#include "delay.hpp"
#include "logger.hpp"
#include "ringbuffer.hpp"
#include "uart_logger_backend.hpp"

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

    // Initialize PIT channel[0] as millisec scheduler
    Drivers::Pit pit(clock.getBusClock());
    pit.init();
    pit.start();
    LOG_DEBUG("PIT OK");
    Services::Delay delayLED1(pit);
    Services::Delay delayLED2(pit);
    Services::Delay delayLED3(pit);
    LOG_DEBUG("DELAY OK");

    gpio.LED_On();
    delayLED1.waitMs(2000);
    gpio.LED_Off();

    delayLED1.startPeriodicMs(1000);
    delayLED2.startPeriodicMs(2000);
    delayLED3.startPeriodicMs(3000);

    while (1) {
        if (delayLED1.expired()) {
            gpio.LED_Toggle(LedColor::Red);
        }
        if (delayLED2.expired()) {
            gpio.LED_Toggle(LedColor::Green);
        }
        if (delayLED3.expired()) {
            gpio.LED_Toggle(LedColor::Blue);
        }
    }
}