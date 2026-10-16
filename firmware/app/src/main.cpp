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

    // CONFIGURATION
    Drivers::Dma dma0(Drivers::Dma::Channel::Channel0);
    Drivers::Dma dma1(Drivers::Dma::Channel::Channel1);

    dma0.init();
    dma1.init();

    uint8_t srcDma[16] = {};
    uint8_t dst0[16] = {};
    uint8_t dst1[16] = {};

    for (uint8_t i = 0; i < 16U; ++i) {
        srcDma[i] = i;
    }

    const bool configured0 = dma0.configLinearTransfer(srcDma, dst0, 1, 1, 16);

    const bool configured1 = dma1.configLinearTransfer(srcDma, dst1, 1, 1, 16);

    const bool linkedMinor = dma0.configureMinorLoopLink(Drivers::Dma::Channel::Channel1);
    const bool linkedMajor = dma0.configureMajorLoopLink(Drivers::Dma::Channel::Channel1);

    // EXECUTION
    if (configured0 && configured1 && linkedMinor && linkedMajor) {
        for (uint8_t i = 0; i < 16U; ++i) {
            const uint16_t before0 = DMA0->TCD[0].CITER_ELINKYES & DMA_CITER_ELINKYES_CITER_MASK;

            const uint16_t before1 = DMA0->TCD[1].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK;

            dma0.start();

            uint32_t timeout = 1000000U;

            while (timeout > 0U) {
                const uint16_t after0 = DMA0->TCD[0].CITER_ELINKYES & DMA_CITER_ELINKYES_CITER_MASK;

                const uint16_t after1 = DMA0->TCD[1].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK;

                if (after0 != before0 && after1 != before1) {
                    break;
                }

                if (dma0.hasError() || dma1.hasError()) {
                    break;
                }

                --timeout;
            }

            if (dma0.hasError() || dma1.hasError()) {
                LOG_ERROR("DMA hardware error");
                break;
            }

            if (timeout == 0U) {
                LOG_ERROR("DMA linking timeout");
                break;
            }
        }
    }

    for (uint8_t i = 0; i < 16U; ++i) {
        LOG_DEBUG("dst0[%d] = %d \t | \t dst1[%d] = %d", i, dst0[i], i, dst1[i]);
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