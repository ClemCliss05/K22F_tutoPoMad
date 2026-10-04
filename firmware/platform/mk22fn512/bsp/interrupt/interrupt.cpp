#include "interrupt.hpp"
#include "MK22FN512.h"

namespace Interrupt {

// =============================================================================
// Shared interrupt state
// =============================================================================

volatile uint32_t pitInterruptCount[4] = {0U, 0U, 0U, 0U};

volatile uint16_t sinOutput = 0U;

// FTM2
volatile uint32_t ftm2OverflowCount = 0U;
volatile uint32_t ftm2CaptureTime = 0U;
volatile bool ftm2CaptureLevel = false;
volatile bool ftm2CapturePending = false;

} // namespace Interrupt

// =============================================================================
// PIT callback storage
// =============================================================================

namespace {

// One callback and one context per PIT channel.
Interrupt::PitCallback pitCallbacks[4] = {nullptr, nullptr, nullptr, nullptr};

void *pitCallbackContexts[4] = {nullptr, nullptr, nullptr, nullptr};

} // anonymous namespace

// =============================================================================
// PIT callback management
// =============================================================================

void Interrupt::setPitCallback(uint8_t channel, PitCallback callback, void *context) {
    if (channel >= 4U) {
        return;
    }

    pitCallbacks[channel] = callback;
    pitCallbackContexts[channel] = context;
}

void Interrupt::handlePitCallback(uint8_t channel) {
    if (channel >= 4U) {
        return;
    }

    if (pitCallbacks[channel] != nullptr) {
        pitCallbacks[channel](pitCallbackContexts[channel]);
    }
}

// =============================================================================
// PIT interrupt handlers
// =============================================================================

extern "C" void PIT0_IRQHandler() {
    if ((PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) != 0U) {
        // Clear interrupt flag.
        PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

        // Count PIT0 interrupts.
        ++Interrupt::pitInterruptCount[0];

        // PIT0 currently has no callback.
    }
}

extern "C" void PIT1_IRQHandler() {
    if ((PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) != 0U) {
        // Clear interrupt flag.
        PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;

        // Count PIT1 interrupts.
        ++Interrupt::pitInterruptCount[1];

        // Execute the function registered for PIT1.
        Interrupt::handlePitCallback(1U);
    }
}

extern "C" void PIT2_IRQHandler() {
    if ((PIT->CHANNEL[2].TFLG & PIT_TFLG_TIF_MASK) != 0U) {
        // Clear interrupt flag.
        PIT->CHANNEL[2].TFLG = PIT_TFLG_TIF_MASK;

        ++Interrupt::pitInterruptCount[2];

        Interrupt::handlePitCallback(2U);
    }
}

extern "C" void PIT3_IRQHandler() {
    if ((PIT->CHANNEL[3].TFLG & PIT_TFLG_TIF_MASK) != 0U) {
        // Clear interrupt flag.
        PIT->CHANNEL[3].TFLG = PIT_TFLG_TIF_MASK;

        ++Interrupt::pitInterruptCount[3];

        Interrupt::handlePitCallback(3U);
    }
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

        // Read signal level at the moment the interrupt is handled.
        const bool level = (GPIOB->PDIR & (1U << 18U)) != 0U;

        Interrupt::ftm2CaptureTime = timestamp;
        Interrupt::ftm2CaptureLevel = level;
        Interrupt::ftm2CapturePending = true;

        // Clear capture flag.
        FTM2->CONTROLS[0].CnSC &= ~FTM_CnSC_CHF_MASK;
    }
}