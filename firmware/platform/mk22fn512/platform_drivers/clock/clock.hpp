#pragma once

#include <cstdint>

namespace Platform::Clock
{
    // initOSC() must be called before setXXXMHz()
    bool initOSC();    
    bool set120MHz();

    uint32_t getCoreClock();
}