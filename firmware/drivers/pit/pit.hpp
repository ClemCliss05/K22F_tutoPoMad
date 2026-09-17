#pragma once

#include <cstdint>

namespace Drivers {

class Pit {
  public:
    explicit Pit(uint32_t clockHz);

    void init();

    /**
     * ticks = PITclkFreq × wantedDuration
     *
     * Example PITclkFreq at 48 MHz:
     *   1 ms  → 48,000 ticks
     *   10 ms → 480,000 ticks
     */
    bool start(uint64_t ticks);

    void stop();

    /**
     * Returns true when the configured timer period has elapsed.
     */
    bool expired() const;

    /**
     * Clear the timer interrupt flag (TIF).
     */
    void clearFlag();

    uint32_t getClockHz() const;

  private:
    uint32_t clockHz_;
};

} // namespace Drivers