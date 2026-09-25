#include "interrupt.hpp"
#include "MK22FN512.h"

// PIT0
volatile uint32_t pit0Ticks = 0U;

// FTM2
volatile uint32_t ftm2OverflowCount = 0U;
volatile uint32_t ftm2PressTime = 0U;
volatile uint32_t ftm2ReleaseTime = 0U;
volatile bool ftm2MeasurementReady = false;

extern "C" void PIT0_IRQHandler() {
    if ((PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) != 0U) {
        PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

        ++pit0Ticks;
    }
}

extern "C" void FTM2_IRQHandler() {
    /*
     * Check timer overflow.
     */
    if ((FTM2->SC & FTM_SC_TOF_MASK) != 0U) {
        ++ftm2OverflowCount;

        FTM2->SC &= ~FTM_SC_TOF_MASK;
    }

    /*
     * Check channel 0 capture.
     */
    if ((FTM2->CONTROLS[0].CnSC & FTM_CnSC_CHF_MASK) != 0U) {
        const uint16_t captureValue = FTM2->CONTROLS[0].CnV;

        /*
         * Build a software 32-bit timestamp.
         */
        const uint32_t timestamp =
            (static_cast<uint32_t>(ftm2OverflowCount) << 16U)
            | static_cast<uint32_t>(captureValue);

        /*
         * Clear capture flag.
         */
        FTM2->CONTROLS[0].CnSC &= ~FTM_CnSC_CHF_MASK;

        /*
         * PTB18 LOW = press.
         * PTB18 HIGH = release.
         */
        if ((GPIOB->PDIR & (1U << 18)) == 0U) {
            ftm2PressTime = timestamp;
        } else {
            ftm2ReleaseTime = timestamp;
            ftm2MeasurementReady = true;
        }
    }
}