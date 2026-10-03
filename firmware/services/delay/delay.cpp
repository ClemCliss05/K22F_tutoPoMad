#include "delay.hpp"

namespace Services {

Delay::Delay(Drivers::PitSystem &pit)
    : pit_(pit), startTick_(0U), durationTicks_(0U), active_(false), periodic_(false) {}

bool Delay::startMs(uint32_t milliseconds) {
    if (milliseconds == 0U) {
        return false;
    }

    startTick_ = pit_.getInterruptCount();
    durationTicks_ = milliseconds;

    active_ = true;
    periodic_ = false;

    return true;
}

bool Delay::startPeriodicMs(uint32_t milliseconds) {
    if (milliseconds == 0U) {
        return false;
    }

    startTick_ = pit_.getInterruptCount();
    durationTicks_ = milliseconds;

    active_ = true;
    periodic_ = true;

    return true;
}

void Delay::waitMs(uint32_t milliseconds) {
    if (milliseconds == 0U) {
        return;
    }

    const uint32_t startTick = pit_.getInterruptCount();

    while ((pit_.getInterruptCount() - startTick) < milliseconds) {
    }
}

bool Delay::expired() {
    if (!active_) {
        return false;
    }

    const uint32_t elapsed = pit_.getInterruptCount() - startTick_;

    if (elapsed < durationTicks_) {
        return false;
    }

    if (periodic_) {
        /*
         * Advance by exactly one period instead of using
         * the current tick as the new starting point.
         *
         * This prevents long-term drift if expired() is
         * called slightly after the expected expiration time.
         */
        startTick_ += durationTicks_;
    } else {
        /*
         * One-shot delay:
         * keep the timer active until the user explicitly
         * calls stop() or starts another delay.
         */
    }

    return true;
}

void Delay::stop() {
    active_ = false;
}

} // namespace Services