#pragma once

#include <cstdint>

namespace Core::Time {
/**
 * Convert timer ticks to microseconds, rounding up.
 */
inline uint64_t ticksToMicroseconds(uint64_t ticks, uint32_t clockHz) {
    if (clockHz == 0U) {
        return 0U;
    }

    return (ticks * 1'000'000ULL + clockHz - 1ULL) / clockHz;
}

/**
 * Convert microseconds to timer ticks, rounding up.
 */
inline uint64_t microsecondsToTicks(uint64_t microseconds, uint32_t clockHz) {
    if (clockHz == 0U) {
        return 0U;
    }

    return (microseconds * clockHz + 999'999ULL) / 1'000'000ULL;
}

} // namespace Core::Time