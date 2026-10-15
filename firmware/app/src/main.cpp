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

    Drivers::Dma dma0(Drivers::Dma::Channel::Channel0);
    Drivers::Dma dma1(Drivers::Dma::Channel::Channel1);
    dma0.init();
    dma1.init();
    uint8_t srcDma[16] = {0};
    uint8_t dstDma[32] = {0};
    for (uint8_t i = 0; i < 16; i++) {
        srcDma[i] = i;
    }

    dma0.configLinearTransfer(srcDma, dstDma, 1, 1, 16);
    dma1.configLinearTransfer(srcDma, (dstDma + 31), -1, 1, 16);

    dma0.configureMajorLoopLink(Drivers::Dma::Channel::Channel1);
    for (uint8_t j = 0; j < 16; j++) {
        dma0.start();
        for (uint8_t n = 0; n < 32; n++) {
            LOG_DEBUG("dstDma[%d] = %d", n, dstDma[n]);
        }
    }
    for (uint8_t k = 0; k < (16 - 1); k++) {
        dma1.start();
        for (uint8_t n = 0; n < 32; n++) {
            LOG_DEBUG("dstDma[%d] = %d", n, dstDma[n]);
        }
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
    delay.startPeriodicMs(2);

    while (1) {
        // In reality, the CPU can sleep:
        __WFI();
    }
}