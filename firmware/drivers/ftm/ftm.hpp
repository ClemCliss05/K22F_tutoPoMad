#pragma once

#include <cstdint>

namespace Drivers
{
    class Ftm
    {
    public:

        struct Capture
        {
            uint32_t timestamp;
            bool level;
        };

        explicit Ftm(uint32_t busClockHz_);

        void init();

        uint32_t getClockHz() const;

        bool captureAvailable() const;

        bool readCapture(Capture& capture);

        uint32_t ticksToMs(uint32_t ticks) const;

    private:

        uint32_t busClockHz_;
    };
}