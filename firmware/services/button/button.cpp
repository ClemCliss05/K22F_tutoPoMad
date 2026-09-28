#include "button.hpp"

namespace Services
{

void Button::init()
{
    // Button idle state = HIGH.
    // LOW = pressed.
    stableState_ = true;
    candidateState_ = true;

    candidateSinceMs_ = 0U;
    candidateTimestamp_ = 0U;

    pressedEvent_ = false;
    releasedEvent_ = false;

    pressTimestamp_ = 0U;
    releaseTimestamp_ = 0U;
}


void Button::update(
    uint32_t nowMs,
    bool captureLevel,
    uint32_t captureTimestamp,
    bool newCapture
)
{
    // -------------------------------------------------------------------------
    // New hardware capture
    // -------------------------------------------------------------------------

    if (newCapture)
    {
        // New candidate state.
        candidateState_ = captureLevel;

        // Start debounce timer.
        candidateSinceMs_ = nowMs;

        // New candidate timestamp.
        candidateTimestamp_ = captureTimestamp;
    }


    // -------------------------------------------------------------------------
    // Debounce
    // -------------------------------------------------------------------------

    if (candidateState_ != stableState_)
    {
        const uint32_t elapsedMs =
            nowMs - candidateSinceMs_;

        if (elapsedMs >= DebounceTimeMs)
        {
            // State is stable for 20 ms.
            stableState_ = candidateState_;

            if (stableState_ == false)
            {
                // LOW = pressed.
                pressTimestamp_ = candidateTimestamp_;
                pressedEvent_ = true;
            }
            else
            {
                // HIGH = released.
                releaseTimestamp_ = candidateTimestamp_;
                releasedEvent_ = true;
            }
        }
    }
}


bool Button::isPressed() const
{
    return !stableState_;
}


bool Button::consumePressed()
{
    if (!pressedEvent_)
    {
        return false;
    }

    pressedEvent_ = false;
    return true;
}


bool Button::consumeReleased()
{
    if (!releasedEvent_)
    {
        return false;
    }

    releasedEvent_ = false;
    return true;
}


uint32_t Button::getPressTimestamp() const
{
    return pressTimestamp_;
}


uint32_t Button::getReleaseTimestamp() const
{
    return releaseTimestamp_;
}

}