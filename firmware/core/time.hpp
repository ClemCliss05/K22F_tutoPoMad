#pragma once

#include <cstdint>

namespace Core::Time {
/**
 * Convert timer ticks to microseconds, rounding to nearest.
 */
inline uint64_t ticksToMicrosec(uint64_t ticks, uint32_t clockHz) {
    if (clockHz == 0U) {
        return 0U;
    }

    return (ticks * 1'000'000ULL + (clockHz / 2U)) / clockHz;
}

/**
 * Convert microseconds to timer ticks, rounding to nearest.
 */
inline uint64_t microsecToTicks(uint64_t us, uint32_t clockHz) {
    if (clockHz == 0U) {
        return 0U;
    }

    return (us * clockHz + 500'000ULL) / 1'000'000ULL;
}

} // namespace Core::Time