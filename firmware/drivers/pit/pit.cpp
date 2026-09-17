#include "pit.hpp"

#include "MK22FN512.h"

namespace Drivers {

Pit::Pit(uint32_t clockHz) : clockHz_(clockHz) {}

void Pit::init() {
    // Enable PIT clock
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    // Disable PIT and reset configuration
    PIT->MCR = 0;
    PIT->CHANNEL[0].TCTRL = 0;

    clearFlag();
}

bool Pit::start(uint64_t ticks) {
    // The PIT counter is 32 bits.
    // A zero tick period is also invalid because LDVAL = ticks - 1.
    if (ticks == 0U || ticks > UINT32_MAX) {
        return false;
    }

    // Stop the timer before changing its configuration.
    stop();

    // PIT counts from LDVAL down to 0.
    // Therefore, ticks periods require LDVAL = ticks - 1.
    PIT->CHANNEL[0].LDVAL = static_cast<uint32_t>(ticks - 1U);

    // Clear any previous timeout flag.
    clearFlag();

    // Start channel 0.
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

    return true;
}

void Pit::stop() {
    PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

bool Pit::expired() const {
    return (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) != 0U;
}

void Pit::clearFlag() {
    // TIF is Write-1-to-Clear
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
}

uint32_t Pit::getClockHz() const {
    return clockHz_;
}

} // namespace Drivers