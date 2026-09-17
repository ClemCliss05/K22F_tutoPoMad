#include "clock.hpp"
#include "interrupt.hpp"

#include "gpio.hpp"
#include "uart.hpp"
#include "pit.hpp"

#include "logger.hpp"
#include "ringbuffer.hpp"
#include "uart_logger_backend.hpp"
#include "delay.hpp"

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

    // Initialize PIT channel[0] with interruptions
    Drivers::Pit pit(clock.getBusClock());
    pit.init();
    LOG_DEBUG("PIT OK");

    pit.start(48000);

    while (1) {
        if(pit0Ticks >= 1000U){
            pit0Ticks = 0;

            gpio.LED_Toggle();
        }
    }
}