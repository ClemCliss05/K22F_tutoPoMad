#include "pit.hpp"
#include "MK22FN512.h"

void Drivers::Pit::init(){
    // Enable PIT clock
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    // Reset PIT configuration
    PIT->MCR = 0x00000000;

    // Set timeout period for the timer interrupts
    // LDVAL trigger = (wantedPeriod*freqClockTimer) - 1
    // ex 1ms Period: 1e(-3)*48000000 - 1 = 47 999 cycles.
    // 47 999 cycles = 0xbb7f
    PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(0xbb7f);

    // // Enable interruption
    // PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

    // Enable Timer
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK;
}