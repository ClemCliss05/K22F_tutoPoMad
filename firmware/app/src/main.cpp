#include "MK22FN512.h"

// BSP platform
#include "clock.hpp"

// Drivers
#include "adc.hpp"
#include "dac.hpp"
#include "dma.hpp"
#include "gpio.hpp"
#include "pdb.hpp"
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

    LOG_INFO("DMA OK");

    // =========================================================================
    // DAC
    // =========================================================================

    Drivers::Dac dac;
    dac.initFifo();

    LOG_INFO("DAC OK");

    // =========================================================================
    // PDB
    // =========================================================================

    // pdbClk = 48e6 / (128 * 40) = 9375
    Drivers::PdbDac pdbDac(clock.getBusClock(), Drivers::Pdb::Prescaler::Div128, Drivers::Pdb::Multiplier::X40);
    pdbDac.init();

    LOG_INFO("PDB OK");
    
    uint16_t waveform[16] = {
        1000, 1500, 2000, 2500,
        3000, 3500, 4000, 3500,
        3000, 2500, 2000, 1500,
        1000,  500,  200,  500
    };

    dma.configureMemoryToPeripheral(
        waveform,
        dac.fifoAddress(),
        16
    );

    for (uint8_t i = 0; i < 16; i++) {
        dma.start();

        LOG_DEBUG(
            "Transfer %d: CITER=%d",
            i + 1,
            dma.getCurrentIteration()
        );
    }

    LOG_DEBUG("DACoutput before trigger from PDB = %d", adc.read());

    if (!pdbDac.start(500000U)) {
        LOG_ERROR("PDB configuration failed");
    }

    LOG_DEBUG("PDB DAC trigger started");

    if(dma.isComplete()) {
        for (uint8_t i = 0; i < 16; i++) {
            uint16_t value =
                static_cast<uint16_t>(DAC0->DAT[i].DATL) |
                (static_cast<uint16_t>(DAC0->DAT[i].DATH) << 8);

            LOG_DEBUG("DAC[%d] = %d", i, value);
        }
    }

    // =========================================================================
    // Main loop
    // =========================================================================

    // // Start of the main loop
    // gpio.LED_On(LedColor::Blue);
    // for (volatile uint32_t i = 0; i < 5000000; i++) {
    //     __NOP();
    // }
    // gpio.LED_Off(LedColor::Blue);
    delay.startPeriodicMs(100);

    while (1) {
        // // In reality, the CPU can sleep:
        // __WFI();
        if(delay.expired()){
            LOG_DEBUG(
                "CNT=%u MOD=%u DACINT=%u C2=0x%02X ADC=%u",
                PDB0->CNT,
                PDB0->MOD,
                PDB0->DAC[0].INT,
                DAC0->C2,
                adc.read()
            );
        }
    }
}