#include "pit.hpp"

#include "MK22FN512.h"
#include "interrupt.hpp"

namespace Drivers {

Pit::Pit(uint32_t busClockHz, uint8_t channel) : busClockHz_(busClockHz), channel_(channel) {}

void Pit::init() {
    // Enable PIT peripheral clock.
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    // Enable PIT module.
    PIT->MCR = 0U;

    // Stop selected channel before configuration.
    PIT->CHANNEL[channel_].TCTRL = 0U;

    // Clear pending interrupt flag.
    PIT->CHANNEL[channel_].TFLG = PIT_TFLG_TIF_MASK;

    // Enable the corresponding IRQ.
    switch (channel_) {
    case 0:
        NVIC_EnableIRQ(PIT0_IRQn);
        break;

    case 1:
        NVIC_EnableIRQ(PIT1_IRQn);
        break;

    case 2:
        NVIC_EnableIRQ(PIT2_IRQn);
        break;

    case 3:
        NVIC_EnableIRQ(PIT3_IRQn);
        break;

    default:
        break;
    }
}

bool Pit::configure(uint64_t ticks) {
    /*
     * LDVAL is 32-bit and represents ticks - 1.
     * Therefore the maximum period is UINT32_MAX + 1 ticks.
     */
    constexpr uint64_t maxTicks = static_cast<uint64_t>(UINT32_MAX) + 1ULL;

    if (ticks == 0U || ticks > maxTicks) {
        return false;
    }

    // Stop the timer before changing its configuration.
    stop();

    PIT->CHANNEL[channel_].LDVAL = static_cast<uint32_t>(ticks - 1U);

    // Clear any pending timeout flag.
    PIT->CHANNEL[channel_].TFLG = PIT_TFLG_TIF_MASK;

    // Enable interrupt and timer.
    PIT->CHANNEL[channel_].TCTRL = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;

    return true;
}

void Pit::stop() {
    PIT->CHANNEL[channel_].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

uint32_t Pit::getInterruptCount() const {
    return Interrupt::pitInterruptCount[channel_];
}

uint32_t Pit::getClockHz() const {
    return busClockHz_;
}

void Pit::setCallback(void (*callback)(void *), void *context) {
    Interrupt::setPitCallback(channel_, callback, context);
}

// -----------------------------------------------------------------------------
// PitSystem
// -----------------------------------------------------------------------------

PitSystem::PitSystem(uint32_t busClockHz) : Pit(busClockHz, 0U) {}

bool PitSystem::start() {
    // PIT0 is the system tick: 1 ms period.
    // Example: PIT clock = 48 MHz
    // 1 ms = 48,000 PIT clock ticks
    // PIT0 generates an interruption every ms
    const uint64_t ticks = (static_cast<uint64_t>(busClockHz_) + 999ULL) / 1000ULL;

    Interrupt::pitInterruptCount[0] = 0U;

    return configure(ticks);
}

// -----------------------------------------------------------------------------
// PitChannel
// -----------------------------------------------------------------------------

PitChannel::PitChannel(uint32_t busClockHz, Channel channel)
    : Pit(busClockHz, static_cast<uint8_t>(channel)) {}

bool PitChannel::startTicks(uint64_t ticks) {
    Interrupt::pitInterruptCount[channel_] = 0U;

    return configure(ticks);
}

uint64_t PitChannel::microsecondsToTicks(uint32_t us) const {
    /*
     * Rounded conversion:
     *
     * ticks = us * clock / 1,000,000
     * Integer division would truncate fractional ticks
     * Adding (1'000'000 - 1) rounds the result up,
     */
    return (static_cast<uint64_t>(us) * static_cast<uint64_t>(busClockHz_) + 999'999ULL) /
           1'000'000ULL;
}

uint32_t PitChannel::ticksToMicroseconds(uint64_t ticks) const {
    /*
     * Rounded conversion:
     *
     * us = ticks * 1,000,000 / clock
     * Integer division would truncate fractional microseconds.
     * Adding (1,000,000 - 1) rounds the result up.
     */
    return static_cast<uint32_t>((ticks * 1'000'000ULL + 999'999ULL) /
                                 static_cast<uint64_t>(busClockHz_));
}

} // namespace Drivers