#pragma once

#include <cstdint>

#include "pit.hpp"

namespace Services {

class Delay {
  public:
    explicit Delay(Drivers::Pit &pit);

    /**
     * Start a one-shot delay.
     *
     * The delay expires once.
     * Call stop() after expiration when the timer is no longer needed.
     *
     * Calling startMs() again restarts the delay.
     */
    bool startMs(uint32_t milliseconds);

    /**
     * Start a periodic delay.
     *
     * The delay automatically restarts after each expiration.
     */
    bool startPeriodicMs(uint32_t milliseconds);

    /**
     * Wait a blocking delay.
     *
     * The delay freezes the program flow.
     */
    void waitMs(uint32_t milliseconds);

    /**
     * Check whether the delay has expired.
     *
     * For a one-shot delay, expiration remains true until stop()
     * or another startMs()/startPeriodicMs() call.
     *
     * For a periodic delay, expiration is automatically handled
     * by advancing the internal start tick.
     */
    bool expired();

    /**
     * Stop the delay.
     */
    void stop();

  private:
    Drivers::Pit &pit_;

    uint32_t startTick_;
    uint32_t durationTicks_;

    bool active_;
    bool periodic_;
};

} // namespace Services