#include "ftm.hpp"
#include "interrupt.hpp"

namespace Drivers {
// ============================================================
// Ftm
// ============================================================

Ftm::Ftm(FTM_Type *ftm, uint32_t busClockHz) : ftm_(ftm), busClockHz_(busClockHz) {}

uint32_t Ftm::getClockHz() const {
    const uint32_t prescaler = ((ftm_->SC & FTM_SC_PS_MASK) >> FTM_SC_PS_SHIFT);

    return busClockHz_ / (1U << prescaler);
}

uint32_t Ftm::ticksToMs(uint32_t ticks) const {
    const uint32_t ftmClockHz = getClockHz();

    return static_cast<uint32_t>((static_cast<uint64_t>(ticks) * 1000ULL) / ftmClockHz);
}

// ============================================================
// FtmInCap
// ============================================================

FtmInCap::FtmInCap(uint32_t busClockHz) : Ftm(FTM2, busClockHz) {}

void FtmInCap::init() {
    // Peripheral clocks
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

    // PTB18 -> FTM2_CH0
    PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[18] |= PORT_PCR_MUX(0x03);

    // Reset FTM configuration
    FTM2->SC = 0U;
    FTM2->CONTROLS[0].CnSC = 0U;
    FTM2->CONTROLS[0].CnV = 0U;

    FTM2->CNTIN = 0U;
    FTM2->CNT = 0U;
    FTM2->MODE = 0U;
    // The counter wraps from 0xFFFF to 0x0000.
    FTM2->MOD = 0xFFFFU;

    // Input Capture:
    // rising + falling edge
    FTM2->CONTROLS[0].CnSC = FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK;

    // FTM clock = bus clock / 16.
    // ex: bus clock = 48 MHz
    // FTM clock = 48e6 / 16 = 3 MHz -> 0,333 µs per timer tick.
    // Overflow -> (MOD - CNTIN + 0x0001) / FTMclk
    //           = (65535 - 0 + 1) / 3MHz
    //           ≈ 21,85 ms
    FTM2->SC = FTM_SC_CLKS(0x01) | FTM_SC_PS(0x04);

    // Enable interrupts
    FTM2->CONTROLS[0].CnSC |= FTM_CnSC_CHIE_MASK;
    FTM2->SC |= FTM_SC_TOIE_MASK;

    NVIC_ClearPendingIRQ(FTM2_IRQn);
    NVIC_EnableIRQ(FTM2_IRQn);
}

bool FtmInCap::captureAvailable() const {
    return Interrupt::ftm2CapturePending;
}

bool FtmInCap::readCapture(Capture &capture) {
    if (!Interrupt::ftm2CapturePending) {
        return false;
    }

    NVIC_DisableIRQ(FTM2_IRQn);

    capture.timestamp = Interrupt::ftm2CaptureTime;

    capture.level = Interrupt::ftm2CaptureLevel;

    Interrupt::ftm2CapturePending = false;

    NVIC_EnableIRQ(FTM2_IRQn);

    return true;
}

// ============================================================
// FtmPwm
// ============================================================

FtmPwm::FtmPwm(uint32_t busClockHz) : Ftm(FTM3, busClockHz) {}

void FtmPwm::init() {
    // Peripheral clocks
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;

    // PTD2 -> FTM3_CH2
    PORTD->PCR[2] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[2] |= PORT_PCR_MUX(0x04);

    // // PTD3 -> FTM3_CH3 (IF NEEDED)
    // PORTD->PCR[3] &= ~PORT_PCR_MUX_MASK;
    // PORTD->PCR[3] |= PORT_PCR_MUX(0x04);

    // Reset FTM3_CH2 configuration
    FTM3->SC = 0U;
    FTM3->CONTROLS[2].CnSC = 0U;
    FTM3->CONTROLS[2].CnV = 0U;

    FTM3->CNTIN = 0U;
    FTM3->CNT = 0U;
    FTM3->MODE = 0U;
    // PWM of 1 KHz with FTMclk = 3 MHz
    // MOD = (FTMclk / PWM) - 1
    // MOD = 3000 - 1 = 2999
    FTM3->MOD = 2999;

    // Edge-Aligned PWM:
    // High-true pulses (clear Output on match)
    FTM3->CONTROLS[2].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    FTM3->CONTROLS[2].CnSC &= ~FTM_CnSC_ELSA_MASK;

    // FTM clock = bus clock / 16
    // ex: 3 MHz = 48 MHz / 16
    FTM3->SC = FTM_SC_CLKS(0x01) | FTM_SC_PS(0x04);
}

void FtmPwm::setDutyCycle(uint8_t dutyPercent) {
    if (dutyPercent > 100U) {
        dutyPercent = 100U;
    }

    const uint32_t periodTicks = static_cast<uint32_t>(ftm_->MOD) + 1U;

    // CnV = dutyPercent * (MOD + 1)
    ftm_->CONTROLS[2].CnV = static_cast<uint16_t>((periodTicks * dutyPercent) / 100U);
}
} // namespace Drivers
