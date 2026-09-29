#pragma once

#include <cstdint>

#include "MK22FN512.h"

namespace Drivers {
class Ftm {
  public:
    /*
     * FTM driver functions
     */

    Ftm(FTM_Type *ftm, uint32_t busClockHz);

    uint32_t getClockHz() const;

    uint32_t ticksToMs(uint32_t ticks) const;

  protected:
    FTM_Type *ftm_;
    uint32_t busClockHz_;
};

class FtmInCap : public Ftm {
  public:
    /*
     * FTM input capture driver functions
     * PTB18 -> FTM2_CH0
     */

    struct Capture {
        uint32_t timestamp;
        bool level;
    };

    explicit FtmInCap(uint32_t busClockHz);

    void init();

    bool captureAvailable() const;

    bool readCapture(Capture &capture);
};

class FtmPwm : public Ftm {
  public:
    /*
     * FTM PWM driver functions
     * PTD2 -> FTM3_CH2
     * PTD3 -> FTM3_CH3
     */

    explicit FtmPwm(uint32_t busClockHz);

    void init();

    void setDutyCycle(uint8_t dutyPercent);
};
} // namespace Drivers