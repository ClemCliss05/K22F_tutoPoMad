#include "pdb.hpp"

#include "MK22FN512.h"

namespace Drivers {

Pdb::Pdb(uint32_t busClockHz) : busClockHz_(busClockHz) {}

void Pdb::init() {
    // Enable PDB clock.
    SIM->SCGC6 |= SIM_SCGC6_PDB_MASK;

    // Reset PDB configuration.
    PDB0->SC = 0U;

    // Continuous mode.
    PDB0->SC |= PDB_SC_CONT_MASK;

    // Prescaler = /128, multiplication factor = 1.
    PDB0->SC |= PDB_SC_PRESCALER(7U);

    // Software trigger.
    PDB0->SC |= PDB_SC_TRGSEL(15U);

    // Load values immediately after LDOK.
    PDB0->SC |= PDB_SC_LDMOD(0U);

    // DAC0 interval trigger.
    PDB0->DAC[0].INTC = PDB_INTC_TOE_MASK;
}

bool Pdb::start(uint32_t periodUs) {
    const uint32_t pdbClockHz = getClockHz();

    const uint64_t ticks =
        (static_cast<uint64_t>(periodUs) * static_cast<uint64_t>(pdbClockHz) + 999'999ULL) /
        1'000'000ULL;

    /*
     * MOD is 16-bit and represents ticks - 1.
     * Therefore the maximum period is UINT16_MAX + 1 ticks.
     */
    constexpr uint32_t maxTicks = static_cast<uint32_t>(UINT16_MAX) + 1ULL;

    if (ticks == 0U || ticks > maxTicks) {
        return false;
    }

    const uint32_t value = static_cast<uint32_t>(ticks - 1U);

    // PDB counter period: reaches MOD, then resets to 0 in continuous mode.
    PDB0->MOD = value & PDB_MOD_MOD_MASK;

    // DAC trigger is generated when the DAC interval counter reaches INT.
    PDB0->DAC[0].INT = value & PDB_INT_INT_MASK;

    // Load buffered values.
    PDB0->SC |= PDB_SC_LDOK_MASK;

    // Enable PDB.
    PDB0->SC |= PDB_SC_PDBEN_MASK;

    // Start/restart counter.
    PDB0->SC |= PDB_SC_SWTRIG_MASK;

    return true;
}

void Pdb::stop() {
    PDB0->SC &= ~PDB_SC_PDBEN_MASK;
}

uint32_t Pdb::getClockHz() const {
    const uint32_t prescaler = (PDB0->SC & PDB_SC_PRESCALER_MASK) >> PDB_SC_PRESCALER_SHIFT;

    const uint32_t mult = (PDB0->SC & PDB_SC_MULT_MASK) >> PDB_SC_MULT_SHIFT;

    constexpr uint32_t multFactors[] = {1U, 10U, 20U, 40U};

    const uint32_t prescalerDivider = 1U << prescaler;

    const uint32_t divider = prescalerDivider * multFactors[mult];

    return busClockHz_ / divider;
}

} // namespace Drivers