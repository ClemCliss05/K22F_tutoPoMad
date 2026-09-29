#include "interrupt.hpp"
#include "MK22FN512.h"

namespace Interrupt {
// -------------------------------------------------------------------------
// Shared interrupt state
// -------------------------------------------------------------------------

volatile uint32_t pit0Ticks = 0U;

volatile uint32_t ftm2OverflowCount = 0U;
volatile uint32_t ftm2CaptureTime = 0U;
volatile bool ftm2CaptureLevel = false;
volatile bool ftm2CapturePending = false;
} // namespace Interrupt

// =============================================================================
// PIT0 interrupt handler
// =============================================================================

extern "C" void PIT0_IRQHandler() {
    // Clear PIT interrupt flag.
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

    // 1 ms system tick.
    ++Interrupt::pit0Ticks;
}

// =============================================================================
// FTM2 interrupt handler
// =============================================================================

extern "C" void FTM2_IRQHandler() {
    // -------------------------------------------------------------------------
    // FTM2 overflow
    // -------------------------------------------------------------------------

    if ((FTM2->SC & FTM_SC_TOF_MASK) != 0U) {
        ++Interrupt::ftm2OverflowCount;

        FTM2->SC &= ~FTM_SC_TOF_MASK;
    }

    // -------------------------------------------------------------------------
    // FTM2 channel 0 capture
    // -------------------------------------------------------------------------

    if ((FTM2->CONTROLS[0].CnSC & FTM_CnSC_CHF_MASK) != 0U) {
        const uint16_t captureValue = FTM2->CONTROLS[0].CnV;

        const uint32_t timestamp = (static_cast<uint32_t>(Interrupt::ftm2OverflowCount) << 16U) |
                                   static_cast<uint32_t>(captureValue);

        // Read the signal level at the moment the interrupt is handled.
        const bool level = (GPIOB->PDIR & (1U << 18U)) != 0U;

        Interrupt::ftm2CaptureTime = timestamp;
        Interrupt::ftm2CaptureLevel = level;
        Interrupt::ftm2CapturePending = true;

        // Clear capture flag.
        FTM2->CONTROLS[0].CnSC &= ~FTM_CnSC_CHF_MASK;
    }
}