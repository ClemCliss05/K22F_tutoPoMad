#include "MK22FN512.h"

// BSP platform
#include "clock.hpp"
#include "interrupt.hpp"

// Drivers
#include "adc.hpp"
#include "ftm.hpp"
#include "gpio.hpp"
#include "pit.hpp"
#include "uart.hpp"

// Core
#include "logger.hpp"
#include "ringbuffer.hpp"

// Services
#include "delay.hpp"
#include "uart_logger_backend.hpp"

int main() {
    // =========================================================================
    // Clock
    // =========================================================================

    Bsp::Clock clock;

    clock.initOSC();
    clock.set48MHz();

    // =========================================================================
    // GPIO
    // =========================================================================

    Drivers::Gpio gpio;

    gpio.LED_Init();
    gpio.PBs_Init();

    using LedColor = Drivers::Gpio::LedColor;

    // =========================================================================
    // UART + Logger
    // =========================================================================

    Drivers::Uart uart;
    uart.init();

    Services::UartLoggerBackend uartBackend(uart);

    char loggerBuffer[128];

    RingBuffer ringBuffer(loggerBuffer, sizeof(loggerBuffer));

    Logger logger(ringBuffer, uartBackend);

    LOG_DEBUG("UART OK");

    // =========================================================================
    // ADC0
    // =========================================================================

    Drivers::Adc adc;

    adc.init();

    LOG_DEBUG("ADC0 OK");

    // =========================================================================
    // PIT0
    // =========================================================================

    Drivers::Pit pit(clock.getBusClock());

    pit.init();
    pit.start();

    LOG_DEBUG("PIT0 OK");

    // =========================================================================
    // DELAY
    // =========================================================================

    Services::Delay delay(pit);

    LOG_DEBUG("DELAY OK");

    // =========================================================================
    // FTM3 PWM
    // =========================================================================

    Drivers::FtmPwm ftmPwm(clock.getBusClock());

    ftmPwm.init();

    LOG_DEBUG("FTM2 OK");
    LOG_INFO("FTM2 CLOCK: %lu", ftmPwm.getClockHz());

    // =========================================================================
    // Main loop
    // =========================================================================

    gpio.LED_On(LedColor::Blue);
    for (volatile uint32_t i = 0; i < 5000000; i++) {
        __NOP();
    }
    gpio.LED_Off(LedColor::Blue);

    while (1) {
        const uint16_t adcValue = adc.read();
        const uint8_t dutyPercent = adc.toPercent(adcValue);

        LOG_INFO("ADC=%u DUTY=%u%%", adcValue, dutyPercent);

        ftmPwm.setDutyCycle(dutyPercent);

        delay.waitMs(20);

        // Test without ADC and a connected LED
        // for (uint8_t duty = 0; duty <= 100; ++duty)
        // {
        //     ftmPwm.setDutyCycle(duty);
        //     delay.waitMs(20);
        // }

        // for (int duty = 99; duty > 0; --duty)
        // {
        //     ftmPwm.setDutyCycle(static_cast<uint8_t>(duty));
        //     delay.waitMs(20);
        // }
    }
}