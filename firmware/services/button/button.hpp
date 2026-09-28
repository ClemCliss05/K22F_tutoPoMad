#pragma once

#include <cstdint>

namespace Services
{
    class Button
    {
    public:

        static constexpr uint32_t DebounceTimeMs = 20U;

        void init();

        void update(
            uint32_t nowMs,
            bool captureLevel,
            uint32_t captureTimestamp,
            bool newCapture
        );

        bool isPressed() const;

        bool consumePressed();
        
        bool consumeReleased();

        uint32_t getPressTimestamp() const;

        uint32_t getReleaseTimestamp() const;

    private:
        bool stableState_;
        bool candidateState_;

        uint32_t candidateSinceMs_;
        uint32_t candidateTimestamp_;

        bool pressedEvent_;
        bool releasedEvent_;

        uint32_t pressTimestamp_;
        uint32_t releaseTimestamp_;
    };
}