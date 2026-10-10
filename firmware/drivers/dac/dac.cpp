#include "dac.hpp"
#include "MK22FN512.h"

namespace Drivers {

void Dac::init() {
    // Enable the DAC0 peripheral clock.
    SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;

    // Disable the DAC before changing its configuration.
    DAC0->C0 = 0U;

    // Reset buffer control and pointer configuration.
    DAC0->C1 = 0U;
    DAC0->C2 = 0U;

    // Select VDDA as the reference and enable the DAC.
    DAC0->C0 = DAC_C0_DACRFS_MASK | DAC_C0_DACEN_MASK;
}

void Dac::initNormalBuffer() {
    // Initialize the common DAC configuration.
    init();

    // Enable the buffer in normal mode.
    DAC0->C1 = DAC_C1_DACBFEN_MASK | DAC_C1_DACBFMD(0U);

    // Select the upper buffer index.
    DAC0->C2 = DAC_C2_DACBFUP(15U);
}

void Dac::initFifo() {
    // Initialize the common DAC configuration.
    init();

    // Enable the buffer in FIFO mode.
    DAC0->C1 = DAC_C1_DACBFEN_MASK | DAC_C1_DACBFMD(3U);
}

bool Dac::write(uint16_t value, uint8_t index) {
    if (index > 15U) {
        return false;
    }

    // Keep the value within the 12-bit DAC range.
    value &= MaxValue;

    // V out = V in * (1 + DACDAT0[11:0])/4096
    // DATL ← value [7:0]
    DAC0->DAT[index].DATL = static_cast<uint8_t>(value);
    // DATH ← value [11:8]
    DAC0->DAT[index].DATH = static_cast<uint8_t>(value >> 8U);

    return true;
}

volatile uint16_t *Dac::buffAddress() {
    // Return the address used by the DMA to write FIFO samples.
    return reinterpret_cast<volatile uint16_t *>(&DAC0->DAT[0].DATL);
}

void Dac::enableSoftwareTrigger() {
    // Select software triggering.
    DAC0->C0 |= DAC_C0_DACTRGSEL_MASK;
}

void Dac::softwareTrigger() {
    // Request a software-triggered conversion.
    DAC0->C0 |= DAC_C0_DACSWTRG_MASK;
}

} // namespace Drivers