#include "pit.hpp"

#include "MK22FN512.h"
#include "interrupt.hpp"

namespace Drivers {

Pit::Pit(uint32_t clockHz) : clockHz_(clockHz) {}

void Pit::init() {
    // Enable PIT clock.
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    // Disable PIT and reset configuration.
    PIT->MCR = 0;
    PIT->CHANNEL[0].TCTRL = 0;

    // Clear any pending interrupt flag.
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

    // Enable PIT channel 0 interrupt in the NVIC.
    NVIC_EnableIRQ(PIT0_IRQn);
}

bool Pit::start() {
    // The PIT must generate one interrupt every 1 ms.
    //
    // Example:
    // PIT clock = 48 MHz
    // 1 ms = 48,000 PIT clock ticks
    //
    // PIT counts from LDVAL down to 0, therefore:
    // LDVAL = number_of_ticks - 1
    const uint64_t ticks = (static_cast<uint64_t>(clockHz_) + 999ULL) / 1000ULL;

    if (ticks == 0U || ticks > UINT32_MAX) {
        return false;
    }

    // Stop the timer before changing its configuration.
    stop();

    // Configure the 1 ms period.
    PIT->CHANNEL[0].LDVAL = static_cast<uint32_t>(ticks - 1U);

    // Clear any pending timeout flag.
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

    // Reset the software tick counter.
    pit0Ticks = 0U;

    // Enable PIT channel 0 interrupt.
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

    // Start channel 0.
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

    return true;
}

void Pit::stop() {
    PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

uint32_t Pit::getTicks() const {
    return pit0Ticks;
}

uint32_t Pit::getClockHz() const {
    return clockHz_;
}

} // namespace Drivers