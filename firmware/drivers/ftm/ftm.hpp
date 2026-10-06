#pragma once

#include <cstdint>

#include "MK22FN512.h"

namespace Drivers {
/**
 * Provides common configuration for the K22F FTM timer modules.
 */
class Ftm {
  public:
    enum class Prescaler : uint8_t {
        Div1 = 0U,
        Div2 = 1U,
        Div4 = 2U,
        Div8 = 3U,
        Div16 = 4U,
        Div32 = 5U,
        Div64 = 6U,
        Div128 = 7U
    };

    Ftm(FTM_Type *ftm, uint32_t busClockHz, Prescaler prescaler);

    uint32_t getClockHz() const;
    uint32_t ticksToMs(uint32_t ticks) const;

  protected:
    FTM_Type *ftm_;
    uint32_t busClockHz_;
    Prescaler prescaler_;
};

/**
 * Configures an FTM channel for input capture timing measurements.
 */
class FtmInCap : public Ftm {
  public:
    struct Capture {
        uint32_t timestamp;
        bool level;
    };

    explicit FtmInCap(uint32_t busClockHz, Prescaler prescaler = Prescaler::Div16);

    void init();

    bool captureAvailable() const;
    bool readCapture(Capture &capture);
};

/**
 * Configures an FTM channel to generate PWM signals.
 */
class FtmPwm : public Ftm {
  public:
    explicit FtmPwm(uint32_t busClockHz, Prescaler prescaler = Prescaler::Div16);

    // Pin PTD2 -> FTM3_CH2.
    void init();

    void setDutyCycle(uint8_t dutyPercent);
};

} // namespace Drivers