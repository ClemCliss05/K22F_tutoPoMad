#include "ftm.hpp"

#include "MK22FN512.h"

namespace Drivers {

Ftm::Ftm(uint32_t clockHz) : clockHz_(clockHz) {}

void Ftm::init() {
    // Enable clocks.
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

    // PTB18 -> FTM2_CH0
    PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[18] |= PORT_PCR_MUX(0x03);

    // Reset FTM configuration.
    FTM2->SC = 0;
    FTM2->CONTROLS[0].CnSC = 0;
    FTM2->CONTROLS[0].CnV = 0;

    // Configure counter.
    FTM2->CNTIN = 0U;
    FTM2->CNT = 0U;
    FTM2->MODE = 0;

    // Use the full 16-bit counter range.
    // The counter wraps from 0xFFFF to 0x0000.
    FTM2->MOD = 0xFFFF;

    // Capture rising and falling edges.
    FTM2->CONTROLS[0].CnSC = FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK;

    // Clear any pending capture flag.
    FTM2->CONTROLS[0].CnSC &= ~FTM_CnSC_CHF_MASK;
    FTM2->SC &= ~FTM_SC_TOF_MASK;

    // FTM clock = bus clock / 16.
    // ex: bus clock = 48 MHz
    // FTM clock = 48e6 / 16 = 3 MHz -> 0,333 µs per timer tick.
    // Overflow -> (MOD - CNTIN + 0x0001) / FTMclk
    //           = (65535 - 0 + 1) / 3MHz
    //           ≈ 21,85 ms
    FTM2->SC = FTM_SC_CLKS(0x01) | FTM_SC_PS(0x04);

    // Enable capture and overflow interrupt.
    FTM2->CONTROLS[0].CnSC |= FTM_CnSC_CHIE_MASK;
    FTM2->SC |= FTM_SC_TOIE_MASK;

    // Remove any pending NVIC request generated
    // during configuration.
    NVIC_ClearPendingIRQ(FTM2_IRQn);
    NVIC_EnableIRQ(FTM2_IRQn);
}

uint32_t Ftm::getClockHz() const {
    const uint32_t prescaler = ((FTM2->SC & FTM_SC_PS_MASK) >> FTM_SC_PS_SHIFT);

    return clockHz_ / (1U << prescaler);
}

} // namespace Drivers