#include "MK22FN512.h"

// BSP platform
#include "clock.hpp"
#include "interrupt.hpp"
#include "dwt.hpp"

// Drivers
#include "dac.hpp"
#include "adc.hpp"
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
    // DWT
    // =========================================================================

    Bsp::Dwt dwt;

    dwt.init();

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
    // DAC0
    // =========================================================================

    Drivers::Dac dac;
    dac.init();

    LOG_DEBUG("DAC0 OK");

    // =========================================================================
    // ADC0
    // =========================================================================

    Drivers::Adc adc;
    adc.init();

    LOG_DEBUG("ADC0 OK");

    // =========================================================================
    // PIT0 SYSTEM
    // =========================================================================

    Drivers::PitSystem pitSystem(clock.getBusClock());

    pitSystem.init();
    pitSystem.start();

    LOG_DEBUG("PIT0 SYSTEM OK");

    // =========================================================================
    // DELAY
    // =========================================================================

    Services::Delay delay(pitSystem);

    LOG_DEBUG("DELAY OK");

    // =========================================================================
    // PIT1 DAC
    // =========================================================================

    Drivers::PitChannel pitDac(clock.getBusClock(),
        Drivers::PitChannel::Channel::Channel1);

    pitDac.init();
    pitDac.startTicks(pitDac.microsecondsToTicks(200));

    LOG_DEBUG("PIT1 DAC OK");

    // =========================================================================
    // Main loop
    // =========================================================================

    // Start of the main loop
    gpio.LED_On(LedColor::Blue);
    for (volatile uint32_t i = 0; i < 5000000; i++) {
        __NOP();
    }
    gpio.LED_Off(LedColor::Blue);

    while (1) {
            // Set DAC output
            dac.write(Interrupt::sinOutput);

            LOG_INFO("DAC value: %d", adc.read());
    }
}