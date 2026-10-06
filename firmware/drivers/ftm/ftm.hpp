#pragma once

#include <cstdint>

#include "MK22FN512.h"

namespace Drivers {

class Ftm
{
public:
    enum class Prescaler : uint8_t
    {
        Div1   = 0U,
        Div2   = 1U,
        Div4   = 2U,
        Div8   = 3U,
        Div16  = 4U,
        Div32  = 5U,
        Div64  = 6U,
        Div128 = 7U
    };

    Ftm(FTM_Type* ftm,
        uint32_t busClockHz,
        Prescaler prescaler);

    uint32_t getClockHz() const;
    uint32_t ticksToMs(uint32_t ticks) const;

protected:
    FTM_Type* ftm_;
    uint32_t busClockHz_;
    Prescaler prescaler_;
};

// -----------------------------------------------------------------------------
// FtmInCap
// -----------------------------------------------------------------------------

class FtmInCap : public Ftm
{
public:
    struct Capture
    {
        uint32_t timestamp;
        bool level;
    };

    explicit FtmInCap(
        uint32_t busClockHz,
        Prescaler prescaler = Prescaler::Div16
    );

    void init();

    bool captureAvailable() const;
    bool readCapture(Capture& capture);
};

// -----------------------------------------------------------------------------
// FtmPwm
// -----------------------------------------------------------------------------

class FtmPwm : public Ftm
{
public:
    explicit FtmPwm(
        uint32_t busClockHz,
        Prescaler prescaler = Prescaler::Div16
    );

    void init();

    void setDutyCycle(uint8_t dutyPercent);
};

} // namespace Drivers