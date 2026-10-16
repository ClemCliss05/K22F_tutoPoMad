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
#include "time.hpp"

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
    // UART + Logger + RingBuffer
    // =========================================================================

    Drivers::Uart uart;
    uart.init();

    Services::UartLoggerBackend uartBackend(uart);

    char loggerBuffer[128];

    Core::RingBuffer ringBuffer(loggerBuffer, sizeof(loggerBuffer));

    Core::Logger logger(ringBuffer, uartBackend);

    LOG_INFO("UART OK");

    // =========================================================================
    // PIT0 SYSTEM
    // =========================================================================

    Drivers::PitSystem pitSystem(clock.getBusClock());

    pitSystem.init();
    pitSystem.start();

    LOG_INFO("PIT0 SYSTEM OK");

    // =========================================================================
    // DELAY
    // =========================================================================

    Services::Delay delay(pitSystem);

    LOG_INFO("DELAY OK");

    // =========================================================================
    // DMA
    // =========================================================================

    // Scatter-gather test buffers.
    uint8_t srcA[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t srcB[8] = {10, 11, 12, 13, 14, 15, 16, 17};
    uint8_t srcC[8] = {20, 21, 22, 23, 24, 25, 26, 27};

    uint8_t dstA[8] = {};
    uint8_t dstB[8] = {};
    uint8_t dstC[8] = {};

    Drivers::Dma::ScatterGatherDescriptor descriptorB{};
    Drivers::Dma::ScatterGatherDescriptor descriptorC{};

    Drivers::Dma dma0(Drivers::Dma::Channel::Channel0);
    dma0.init();

    const bool configuredA = dma0.configLinearTransfer(srcA, dstA, 1, 1, 8);

    const bool configuredB =
        dma0.configureScatterGatherDescriptor(descriptorB, srcB, dstB, 1, 1, 0, 0, 1, 8);

    const bool configuredC =
        dma0.configureScatterGatherDescriptor(descriptorC, srcC, dstC, 1, 1, 0, 0, 1, 8);

    const bool linkedBC = dma0.linkScatterGatherDescriptors(descriptorB, descriptorC);

    const bool linkedAB = dma0.configureScatterGather(descriptorB);

    if (configuredA && configuredB && configuredC && linkedBC && linkedAB) {

        uint32_t timeout = 1'000'000U;

        while (dstC[7] != srcC[7] && !dma0.hasError() && timeout > 0U) {

            dma0.start();
            --timeout;
        }
    }

    for (uint8_t i = 0; i < 8U; ++i) {
        LOG_DEBUG("dstA[%d] = %d \t | \t dstB[%d] = %d \t | \t dstC[%d] = %d", i, dstA[i], i,
                  dstB[i], i, dstC[i]);
    }

    LOG_INFO("DMA OK");

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