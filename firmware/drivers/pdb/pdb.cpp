#include "pdb.hpp"

#include "MK22FN512.h"

namespace Drivers {

// -----------------------------------------------------------------------------
// Pdb
// -----------------------------------------------------------------------------

Pdb::Pdb(uint32_t busClockHz, Prescaler prescaler, Multiplier multiplier)
    : busClockHz_(busClockHz), prescaler_(prescaler), multiplier_(multiplier) {}

void Pdb::init() {
    // Enable PDB peripheral clock.
    SIM->SCGC6 |= SIM_SCGC6_PDB_MASK;

    // Reset PDB configuration.
    PDB0->SC = 0U;

    // Continuous mode.
    PDB0->SC |= PDB_SC_CONT_MASK;

    // Configure prescaler and multiplication factor.
    PDB0->SC |= PDB_SC_PRESCALER(static_cast<uint32_t>(prescaler_));

    PDB0->SC |= PDB_SC_MULT(static_cast<uint32_t>(multiplier_));

    // Software trigger.
    PDB0->SC |= PDB_SC_TRGSEL(15U);

    // Load values immediately after LDOK.
    PDB0->SC |= PDB_SC_LDMOD(0U);
}

bool Pdb::configure(uint64_t ticks) {
    /*
     * MOD is 16-bit and represents ticks - 1.
     * Therefore the maximum period is UINT16_MAX + 1 ticks.
     */
    constexpr uint64_t maxTicks = static_cast<uint64_t>(UINT16_MAX) + 1ULL;

    if (ticks == 0U || ticks > maxTicks) {
        return false;
    }

    // Stop the counter before changing its configuration.
    stop();

    const uint32_t value = static_cast<uint32_t>(ticks - 1U);

    // PDB counter period: reaches MOD, then resets to 0.
    PDB0->MOD = value & PDB_MOD_MOD_MASK;

    // Load buffered values.
    PDB0->SC |= PDB_SC_LDOK_MASK;

    // Enable PDB.
    PDB0->SC |= PDB_SC_PDBEN_MASK;

    // Start/restart counter.
    PDB0->SC |= PDB_SC_SWTRIG_MASK;

    return true;
}

void Pdb::stop() {
    // Disable PDB counter.
    PDB0->SC &= ~PDB_SC_PDBEN_MASK;
}

uint32_t Pdb::getClockHz() const {
    constexpr uint32_t multiplierFactors[] = {1U, 10U, 20U, 40U};

    const uint32_t prescalerDivider = 1U << static_cast<uint32_t>(prescaler_);

    const uint32_t multiplier = multiplierFactors[static_cast<uint32_t>(multiplier_)];

    return busClockHz_ / (prescalerDivider * multiplier);
}

// -----------------------------------------------------------------------------
// PdbDac
// -----------------------------------------------------------------------------

PdbDac::PdbDac(uint32_t busClockHz, Prescaler prescaler, Multiplier multiplier)
    : Pdb(busClockHz, prescaler, multiplier) {}

void PdbDac::init() {
    Pdb::init();

    // Enable DAC0 interval trigger.
    PDB0->DAC[0].INTC = PDB_INTC_TOE_MASK;
}

bool PdbDac::start(uint32_t periodUs) {
    const uint32_t pdbClockHz = getClockHz();

    /*
     * Rounded conversion:
     *
     * ticks = periodUs * clock / 1,000,000
     *
     * Adding 1,000,000 - 1 rounds the result up.
     */
    const uint64_t ticks =
        (static_cast<uint64_t>(periodUs) * static_cast<uint64_t>(pdbClockHz) + 999'999ULL) /
        1'000'000ULL;

    constexpr uint64_t maxTicks = static_cast<uint64_t>(UINT16_MAX) + 1ULL;

    if (ticks == 0U || ticks > maxTicks) {
        return false;
    }

    const uint32_t value = static_cast<uint32_t>(ticks - 1U);

    // DAC trigger is generated when the DAC interval counter reaches INT.
    PDB0->DAC[0].INT = value & PDB_INT_INT_MASK;

    return configure(ticks);
}

} // namespace Drivers