#include "pit.hpp"
#include "MK22FN512.h"

// FOR THE MOMENT ONLY WORKING FOR 48 MHZ BUS CLOCK...

void Drivers::Pit::init(){
    // Enable PIT clock
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    // Reset PIT configuration
    PIT->MCR = 0x00000000;

    // // Enable interruption
    // PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
}

void Drivers::Pit::delayMs(uint32_t ms)
{
    // Set timeout period for the timer interrupts
    // LDVAL trigger = (wantedPeriod*freqClockTimer) - 1
    // ex 1ms Period: 1e(-3)*48000000 - 1 = 47 999 cycles.
    // 47 999 cycles = 0xbb7f
    PIT->CHANNEL[0].LDVAL = (48000U * ms) - 1U;

    // Clear éventuel timeout précédent
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

    // Start
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

    // Wait
    while (!(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK))
    {
    }

    // Stop
    PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;

    // Clear timeout
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
}