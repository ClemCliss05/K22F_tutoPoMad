#pragma once

#include <cstdint>

#include "MK22FN512.h"

namespace Drivers
{
    class Ftm
    {
    public:
        Ftm(
            FTM_Type* ftm,
            uint32_t busClockHz
        );

        uint32_t getClockHz() const;

        uint32_t ticksToMs(uint32_t ticks) const;

    protected:
        FTM_Type* ftm_;
        uint32_t busClockHz_;
    };


    class FtmInCap : public Ftm
    {
    public:

        struct Capture
        {
            uint32_t timestamp;
            bool level;
        };

        FtmInCap(
            FTM_Type* ftm,
            uint32_t busClockHz
        );

        void init();

        bool captureAvailable() const;

        bool readCapture(Capture& capture);
    };


    class FtmPwm : public Ftm
    {
    public:

        FtmPwm(
            FTM_Type* ftm,
            uint32_t busClockHz
        );

        void init();

        void setDutyCycle(uint8_t percent);
    };
}