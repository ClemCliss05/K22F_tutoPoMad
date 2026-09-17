#include "MK22FN512.h"

volatile uint32_t pit0Ticks = 0U;

extern "C" void PIT0_IRQHandler()
{
    if ((PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) != 0U)
    {
        // Clear PIT interrupt flag.
        PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

        // Record the interrupt.
        ++pit0Ticks;
    }
}