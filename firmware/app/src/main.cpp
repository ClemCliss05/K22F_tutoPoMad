#include "MK22FN512.h"

#include "clock.hpp"
#include "interrupt.hpp"

#include "ftm.hpp"
#include "gpio.hpp"
#include "pit.hpp"
#include "uart.hpp"

#include "logger.hpp"
#include "ringbuffer.hpp"
#include "uart_logger_backend.hpp"

#include "button.hpp"


int main()
{
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

    RingBuffer ringBuffer(
        loggerBuffer,
        sizeof(loggerBuffer)
    );

    Logger logger(
        ringBuffer,
        uartBackend
    );

    LOG_DEBUG("UART OK");


    // =========================================================================
    // PIT0
    // =========================================================================

    Drivers::Pit pit(clock.getBusClock());

    pit.init();
    pit.start();

    LOG_DEBUG("PIT0 OK");


    // =========================================================================
    // FTM2 Input Capture
    // =========================================================================

    Drivers::Ftm ftm(clock.getBusClock());

    ftm.init();

    LOG_DEBUG("FTM2 OK");


    // =========================================================================
    // Button
    // =========================================================================

    Services::Button button;

    button.init();


    // =========================================================================
    // Main loop
    // =========================================================================

    gpio.LED_On(LedColor::Blue);
    for (volatile uint32_t i = 0; i < 5000000; i++) {
        __NOP();
    }
    gpio.LED_Off(LedColor::Blue);

    while (1)
    {
        Drivers::Ftm::Capture capture{};

        const bool newCapture =
            ftm.readCapture(capture);

        button.update(
            Interrupt::pit0Ticks,
            capture.level,
            capture.timestamp,
            newCapture
        );

        if (button.consumePressed())
        {
            LOG_INFO(
                "BUTTON PRESSED: %lu",
                button.getPressTimestamp()
            );

            gpio.LED_On(LedColor::Red);
        }

        if (button.consumeReleased())
        {
            LOG_INFO(
                "BUTTON RELEASED: %lu",
                button.getReleaseTimestamp()
            );

            const uint32_t durationTicks =
                button.getReleaseTimestamp()
                - button.getPressTimestamp();

            const uint32_t durationMs =
                ftm.ticksToMs(durationTicks);

            LOG_INFO(
                "DURATION MS: %lu",
                durationMs
            );

            gpio.LED_Off(LedColor::Red);
        }
    }
}