#pragma once

#include <cstdint>

namespace Bsp {
class Clock {
  public:
    // initOSC() must be called before setXXXMHz()
    bool initOSC();
    bool set120MHz();
    bool set48MHz();

    uint32_t getCoreClock() const;
    uint32_t getBusClock() const;
};
} // namespace Bsp