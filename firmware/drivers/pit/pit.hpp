#pragma once

#include <cstdint>

namespace Drivers {

/**
 * Base class for configuring a K22F PIT timer channel.
 */
class Pit {
  public:
    void init();
    void stop();

    uint32_t getInterruptCount() const;
    uint32_t getClockHz() const;

    // Register a function called when this PIT channel interrupts.
    void setCallback(void (*callback)(void *), void *context);

  protected:
    Pit(uint32_t busClockHz, uint8_t channel);

    bool configure(uint64_t ticks);

    uint32_t busClockHz_;
    uint8_t channel_;
};

/**
 * Provides the system time base using a periodic PIT interrupt.
 * PIT0 is reserved for the 1 ms system tick.
 */
class PitSystem : public Pit {
  public:
    explicit PitSystem(uint32_t busClockHz);

    bool start();
};

/**
 * Provides a configurable PIT channel for periodic timing operations.
 */
class PitChannel : public Pit {
  public:
    enum class Channel : uint8_t { Channel1 = 1U, Channel2 = 2U, Channel3 = 3U };

    PitChannel(uint32_t busClockHz, Channel channel);

    bool startUs(uint32_t periodUs);
};

} // namespace Drivers