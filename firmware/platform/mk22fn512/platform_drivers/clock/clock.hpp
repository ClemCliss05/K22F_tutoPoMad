#pragma once

#include <cstdint>

namespace Platform::Clock
{
    bool initOSC();    
    bool set120MHz();

    uint32_t getCoreClock();
}