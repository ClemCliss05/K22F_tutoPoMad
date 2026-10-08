#include "MK22FN512.h"

// BSP platform
#include "clock.hpp"

// Drivers
#include "dac.hpp"
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

    LOG_INFO("DMA OK");

    // =========================================================================
    // DAC
    // =========================================================================

    Drivers::Dac dac;
    dac.initFIFO();

    LOG_INFO("DAC OK");

    LOG_INFO(
        "After init: C1=0x%02X C2=0x%02X SR=0x%02X",
        DAC0->C1,
        DAC0->C2,
        DAC0->SR
    );

    dac.writeBuffer(1000);

    LOG_INFO(
        "After 1: C1=0x%02X C2=0x%02X SR=0x%02X",
        DAC0->C1,
        DAC0->C2,
        DAC0->SR
    );
    dac.writeBuffer(2000);
    LOG_INFO(
        "After 2: C1=0x%02X C2=0x%02X SR=0x%02X",
        DAC0->C1,
        DAC0->C2,
        DAC0->SR
    );
    dac.writeBuffer(3000);
    LOG_INFO(
        "After 3: C1=0x%02X C2=0x%02X SR=0x%02X",
        DAC0->C1,
        DAC0->C2,
        DAC0->SR
    );
    dac.writeBuffer(4000);
    LOG_INFO(
        "After 4: C1=0x%02X C2=0x%02X SR=0x%02X",
        DAC0->C1,
        DAC0->C2,
        DAC0->SR
    );

    for(uint8_t i = 0; i < 16; i++){
        LOG_INFO("DAC0->DAT[%d] = %d", i, DAC0->DAT[i]);
    }

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