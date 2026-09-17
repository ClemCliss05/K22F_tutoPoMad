#include "delay.hpp"

namespace Services {

Delay::Delay(Drivers::Pit &pit) : pit_(pit) {}

void Delay::ms(uint32_t milliseconds) {
    const uint64_t ticks = (static_cast<uint64_t>(pit_.getClockHz()) * milliseconds) / 1000ULL;

    pit_.start(static_cast<uint32_t>(ticks));

    while (!pit_.expired()) {
    }

    pit_.clearFlag();
    pit_.stop();
}

void Delay::us(uint32_t microseconds) {
    const uint64_t ticks = (static_cast<uint64_t>(pit_.getClockHz()) * microseconds) / 1'000'000ULL;

    pit_.start(static_cast<uint32_t>(ticks));

    while (!pit_.expired()) {
    }

    pit_.clearFlag();
    pit_.stop();
}

} // namespace Services