#include "dwt.hpp"
#include "MK22FN512.h"

namespace Bsp {

void Dwt::init()
{
    // Enable Cortex-M trace/debug components.
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    // Reset cycle counter.
    DWT->CYCCNT = 0U;

    // Enable DWT cycle counter.
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

uint32_t Dwt::getCycles() const
{
    return DWT->CYCCNT;
}

}