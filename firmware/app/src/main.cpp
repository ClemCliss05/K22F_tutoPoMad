#include "MK22FN512.h"

// BSP platform
#include "clock.hpp"

// Drivers
#include "adc.hpp"
#include "dac.hpp"
#include "dma.hpp"
#include "dmamux.hpp"
#include "gpio.hpp"
#include "pdb.hpp"
#include "pit.hpp"
#include "uart.hpp"

// Core
#include "logger.hpp"
#include "ringbuffer.hpp"

// Services
#include "delay.hpp"
#include "uart_logger_backend.hpp"

// C lib
#include <math.h>

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
    // ADC
    // =========================================================================

    Drivers::Adc adc;
    adc.init();

    LOG_INFO("ADC OK");

    // =========================================================================
    // DMA
    // =========================================================================

    Drivers::Dma dma(Drivers::Dma::Channel::Channel0);
    dma.init();

    LOG_INFO("DMA OK");

    // =========================================================================
    // DMAMUX
    // =========================================================================

    Drivers::DmaMux dmaMux;
    dmaMux.init();

    LOG_INFO("DMAMUX OK");

    // =========================================================================
    // DAC
    // =========================================================================

    Drivers::Dac dac;
    dac.initFifo();
    // dac.initNormalBuffer();

    LOG_INFO("DAC OK");

    // =========================================================================
    // PDB
    // =========================================================================

    // pdbClk = 48e6 / (128 * 40) = 9375
    Drivers::PdbDac pdbDac(clock.getBusClock(), Drivers::Pdb::Prescaler::Div128,
                           Drivers::Pdb::Multiplier::X40);
    pdbDac.init();

    LOG_INFO("PDB OK");

    // =========================================================================
    // Sinus LUT
    // =========================================================================

    constexpr uint16_t LutSize = 64U;
    constexpr float Pi = 3.14159265358979323846f;
    constexpr float Offset = 2047.5f;
    constexpr float Amplitude = 1800.0f;

    alignas(2 * LutSize) uint16_t waveform[LutSize];

    for (uint16_t i = 0; i < LutSize; ++i) {
        const float phase = (2.0f * Pi * static_cast<float>(i)) / static_cast<float>(LutSize);

        const float sample = Offset + Amplitude * sinf(phase);

        waveform[i] = static_cast<uint16_t>(sample + 0.5f);
        LOG_DEBUG("waveform[%d] = %d", i, waveform[i]);
    }

    LOG_INFO("Sinus LUT OK");

    // =========================================================================
    // Sinus Generation
    // =========================================================================

    if (!dma.configCircularTransfer(waveform, dac.buffAddress(), 0U, LutSize, 0U, 1U, 64U)) {
        LOG_ERROR("dma.configCircularTransfer");
    }

    if (!dmaMux.configure(Drivers::Dma::Channel::Channel0, Drivers::DmaMux::Source::Dac0)) {
        LOG_ERROR("dmaMux.configure");
    }

    // Arm the DMA path before starting the PDB.
    dma.enableHardwareRequests();
    dac.configureBufferEvent(Drivers::Dac::BufferEvent::Watermark);
    dac.enableDmaRequests();

    if (!pdbDac.startUs(10000U)) {
        LOG_ERROR("PDB configuration failed");
    }

    LOG_DEBUG("PDB DAC trigger started");

    // =========================================================================
    // Main loop
    // =========================================================================

    // // Start of the main loop
    // gpio.LED_On(LedColor::Blue);
    // for (volatile uint32_t i = 0; i < 5000000; i++) {
    //     __NOP();
    // }
    // gpio.LED_Off(LedColor::Blue);
    delay.startPeriodicMs(2);

    while (1) {
        // // In reality, the CPU can sleep:
        // __WFI();
        if (delay.expired()) {
            LOG_DEBUG("CNT=%u DACINT=%u C2=0x%02X SR=0x%X CITER=0x%X ES=0x%X ADC=%u", PDB0->CNT,
                      PDB0->DAC[0].INT, DAC0->C2, DAC0->SR, DMA0->TCD[0].CITER_ELINKNO, DMA0->ES,
                      adc.read());
        }
    }
}