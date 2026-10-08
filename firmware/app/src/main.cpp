#include "MK22FN512.h"

// BSP platform
#include "clock.hpp"

// Drivers
#include "dma.hpp"
#include "gpio.hpp"
#include "pit.hpp"
#include "uart.hpp"

// Core
#include "logger.hpp"
#include "ringbuffer.hpp"

// Services
#include "uart_logger_backend.hpp"
#include "delay.hpp"

// C lib
#include <math.h>

// namespace {

// float sinAngle = 0.0f;

// void generateSinSample(void *context) {
//     auto *dac = static_cast<Drivers::Dac *>(context);

//     constexpr float STEP = 0.01f;
//     constexpr float TWO_PI = 6.28f;

//     // Advance the phase.
//     sinAngle += STEP;

//     if (sinAngle > TWO_PI) {
//         sinAngle = 0.0f;
//     }

//     // Compute sine.
//     const float y = sinf(sinAngle);

//     // Convert [-1, +1] to unsigned 12-bit DAC range.
//     const uint16_t output = static_cast<uint16_t>(0x07FF + static_cast<int16_t>(0x07FF * y));

//     dac->write(output);
// }

// } // anonymous namespace

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
    // UART + Logger + RingBuffer
    // =========================================================================

    Drivers::Uart uart;
    uart.init();

    Services::UartLoggerBackend uartBackend(uart);

    char loggerBuffer[128];

    RingBuffer ringBuffer(loggerBuffer, sizeof(loggerBuffer));

    Logger logger(ringBuffer, uartBackend);

    LOG_DEBUG("UART OK");

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
    // DMA
    // =========================================================================

    Drivers::Dma dma(Drivers::Dma::Channel::Channel0);
    uint16_t srcBuf[8] = {100, 200, 300, 400, 500, 600, 700, 800};
    uint16_t dstBuf = 0;

    dma.configureMemoryToPeripheral(srcBuf, &dstBuf, 8);

    // VERIFICATION
    while (!dma.isComplete()) {
        dma.start();

        LOG_INFO(
            "CITER = %d, dstBuf = %d",
            dma.getCurrentIteration(),
            dstBuf
        );
    }

    LOG_INFO("DMA transfer completed");

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
        // In reality, the CPU can sleep:
        __WFI();
    }
}