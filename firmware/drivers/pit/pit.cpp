#include "pit.hpp"

#include "MK22FN512.h"

namespace Drivers {

Pit::Pit(uint32_t clockHz) : clockHz_(clockHz) {}

void Pit::init() {
    // Enable PIT clock
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    // Reset PIT CHANNEL[0] configuration
    PIT->MCR = 0;
    PIT->CHANNEL[0].TCTRL = 0;

    // // Enable interruption
    // PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
}

void Pit::start(uint32_t ticks) {
    // Stop the timer before reconfiguration
    stop();

    // PIT counts from LDVAL down to 0
    PIT->CHANNEL[0].LDVAL = ticks - 1U;

    // Clear any previous timeout flag
    clearFlag();

    // Start timer
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
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