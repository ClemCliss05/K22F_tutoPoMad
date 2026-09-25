#include "MK22FN512.h"

#include "clock.hpp"
#include "interrupt.hpp"

#include "ftm.hpp"
#include "gpio.hpp"
#include "uart.hpp"

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

    // Initialize FTM2 channel[0] as Input capture
    Drivers::Ftm ftm(clock.getBusClock());
    ftm.init();
    LOG_DEBUG("FTM OK");

    gpio.LED_On(LedColor::Blue);
    for (volatile uint32_t i = 0; i < 5000000; i++) {
        __NOP();
    }
    gpio.LED_Off(LedColor::Blue);

    while (1) {

        if (gpio.PB1_GetState()) {
            gpio.LED_On(LedColor::Red);
        } else {
            gpio.LED_Off(LedColor::Red);
        }
        
        if (ftm2MeasurementReady) {

            const uint64_t elapsedTicks = static_cast<uint64_t>(ftm2ReleaseTime - ftm2PressTime);

            const uint32_t durationMs =
                static_cast<uint32_t>((elapsedTicks * 1000U) / ftm.getClockHz());

            LOG_INFO("PRESS   = %lu", ftm2PressTime);
            LOG_INFO("RELEASE = %lu", ftm2ReleaseTime);
            LOG_INFO("DURATION = %lu ms", durationMs);
            LOG_INFO("----------");

            ftm2MeasurementReady = false;
        }
    }
}