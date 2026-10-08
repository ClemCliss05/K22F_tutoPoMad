#include "dac.hpp"
#include "MK22FN512.h"

namespace Drivers {
void Dac::init() {
    // Enable DAC0 clock
    SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;

    // Reset DAC configuration
    DAC0->C0 = 0U;
    DAC0->C1 = 0U;
    DAC0->C2 = 0U;

    // Enable DAC0 & selects DACREF_2 = Vdda = 3.3V
    DAC0->C0 |= DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;

    // hardware trigger is selected
    DAC0->C0 &= ~DAC_C0_DACTRGSEL_MASK;
}

void Dac::write(uint16_t value) {
    // 12-bit DAC
    value &= 0x0FFF;

    // V out = V in * (1 + DACDAT0[11:0])/4096
    // DATL ← bits [7:0]
    DAC0->DAT->DATL = value & 0xFF;
    // DATH ← bits [11:8]
    DAC0->DAT->DATH = value >> 8;
}

void Dac::initFIFO() {
    // Enable DAC0 clock.
    SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;

    // Reset DAC configuration.
    DAC0->C0 = 0U;
    DAC0->C1 = 0U;
    DAC0->C2 = 0U;

    // Enable DAC and select VDDA as reference.
    DAC0->C0 =
        DAC_C0_DACEN_MASK |
        DAC_C0_DACRFS_MASK;

    // Enable DAC buffer in FIFO mode.
    DAC0->C1 =
        DAC_C1_DACBFEN_MASK |
        DAC_C1_DACBFMD(3U);
}

void Dac::enableBuffer()
{
    DAC0->C1 =
        DAC_C1_DACBFEN_MASK |
        DAC_C1_DACBFMD(3U);
}

void Dac::writeBuffer(uint16_t value)
{
    value &= MaxValue;

    DAC0->DAT[0].DATL = static_cast<uint8_t>(value);
    DAC0->DAT[0].DATH = static_cast<uint8_t>(value >> 8);
}

void Dac::trigger()
{
    DAC0->C0 |= DAC_C0_DACSWTRG_MASK;
}

} // namespace Drivers