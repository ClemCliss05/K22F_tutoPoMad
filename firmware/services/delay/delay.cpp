#include "delay.hpp"

namespace Services {

Delay::Delay(Drivers::Pit &pit) : pit_(pit) {}

bool Delay::us(uint32_t microseconds) {
    // A zero-duration delay is considered invalid.
    if (microseconds == 0U) {
        return false;
    }

    const uint64_t clockHz = pit_.getClockHz();

    // Convert microseconds to PIT clock ticks.
    //
    // The addition performs a ceiling division so that the actual
    // delay is never shorter than the requested delay.
    const uint64_t ticks = (clockHz * microseconds + 999'999ULL) / 1'000'000ULL;

    // Pit::start() checks whether the requested number of ticks
    // can be represented by the 32-bit PIT counter.
    if (!pit_.start(ticks)) {
        return false;
    }

    // Blocking wait until the PIT reaches zero.
    while (!pit_.expired()) {
    }

    // Clear the timeout flag before stopping the timer.
    pit_.clearFlag();

    // Stop the timer.
    pit_.stop();

    return true;
}

bool Delay::ms(uint32_t milliseconds) {
    // A zero-duration delay is considered invalid.
    if (milliseconds == 0U) {
        return false;
    }

    const uint64_t clockHz = pit_.getClockHz();

    // Convert milliseconds to PIT clock ticks.
    //
    // Ceiling division ensures that the actual delay is never
    // shorter than the requested delay.
    const uint64_t ticks = (clockHz * milliseconds + 999ULL) / 1'000ULL;

    // Pit::start() checks whether the requested number of ticks
    // can be represented by the 32-bit PIT counter.
    if (!pit_.start(ticks)) {
        return false;
    }

    // Blocking wait until the PIT reaches zero.
    while (!pit_.expired()) {
    }

    // Clear the timeout flag before stopping the timer.
    pit_.clearFlag();

    // Stop the timer.
    pit_.stop();

    return true;
}

} // namespace Services