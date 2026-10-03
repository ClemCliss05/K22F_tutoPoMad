#pragma once

#include <cstdint>

namespace Drivers {

/**
 * Base class for all PIT channels.
 *
 * Handles common PIT hardware operations.
 */
class Pit
{
public:
    void init();
    void stop();

    uint64_t getInterruptCount() const;
    uint32_t getClockHz() const;

protected:
    Pit(uint32_t clockHz, uint8_t channel);

    bool configure(uint64_t ticks);

    uint32_t clockHz_;
    uint8_t channel_;
};


/**
 * System PIT.
 *
 * PIT0 is reserved for the 1 ms system tick.
 */
class PitSystem : public Pit
{
public:
    explicit PitSystem(uint32_t clockHz);

    bool start();
};


/**
 * Generic PIT channel.
 *
 * PIT1, PIT2 and PIT3 can be used for application-specific
 * periodic events such as DAC scheduling.
 */
class PitChannel : public Pit
{
public:
    enum class Channel : uint8_t
    {
        Channel1 = 1,
        Channel2 = 2,
        Channel3 = 3
    };

    PitChannel(uint32_t clockHz, Channel channel);

    bool startTicks(uint64_t ticks);

    uint64_t microsecondsToTicks(uint32_t us) const;
    uint32_t ticksToMicroseconds(uint64_t ticks) const;
};

} // namespace Drivers