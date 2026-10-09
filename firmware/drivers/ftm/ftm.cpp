#include "ftm.hpp"

#include "interrupt.hpp"

namespace Drivers {

// -----------------------------------------------------------------------------
// Ftm
// -----------------------------------------------------------------------------

Ftm::Ftm(FTM_Type *ftm, uint32_t busClockHz, Prescaler prescaler)
    : ftm_(ftm), busClockHz_(busClockHz), prescaler_(prescaler) {}

uint32_t Ftm::getClockHz() const {
    const uint32_t divider = 1U << static_cast<uint32_t>(prescaler_);

    return busClockHz_ / divider;
}

uint32_t Ftm::ticksToMs(uint32_t ticks) const {
    const uint32_t ftmClockHz = getClockHz();

    return static_cast<uint32_t>((static_cast<uint64_t>(ticks) * 1000ULL) /
                                 static_cast<uint64_t>(ftmClockHz));
}

// -----------------------------------------------------------------------------
// FtmInCap
// -----------------------------------------------------------------------------

FtmInCap::FtmInCap(uint32_t busClockHz, Prescaler prescaler) : Ftm(FTM2, busClockHz, prescaler) {}

void FtmInCap::init() {
    // Enable peripheral clocks.
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

    // PTB18 -> FTM2_CH0.
    PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[18] |= PORT_PCR_MUX(0x03);

    // Reset FTM configuration.
    FTM2->SC = 0U;
    FTM2->CONTROLS[0].CnSC = 0U;
    FTM2->CONTROLS[0].CnV = 0U;

    FTM2->CNTIN = 0U;
    FTM2->CNT = 0U;
    FTM2->MODE = 0U;

    // Counter wraps from 0xFFFF to 0x0000.
    FTM2->MOD = 0xFFFFU;

    // Input Capture: rising + falling edge.
    FTM2->CONTROLS[0].CnSC = FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK;

    // Select FTM clock and configured prescaler.
    FTM2->SC = FTM_SC_CLKS(0x01U) | FTM_SC_PS(static_cast<uint32_t>(prescaler_));

    // Enable channel and overflow interrupts.
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

// -----------------------------------------------------------------------------
// FtmPwm
// -----------------------------------------------------------------------------

FtmPwm::FtmPwm(uint32_t busClockHz, Prescaler prescaler) : Ftm(FTM3, busClockHz, prescaler) {}

void FtmPwm::init() {
    // Enable peripheral clocks.
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;

    // PTD2 -> FTM3_CH2.
    PORTD->PCR[2] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[2] |= PORT_PCR_MUX(0x04);

    // Stop FTM3 before configuration.
    FTM3->SC = 0U;

    // Reset FTM3_CH2 configuration.
    FTM3->CONTROLS[2].CnSC = 0U;
    FTM3->CONTROLS[2].CnV = 0U;

    FTM3->CNTIN = 0U;
    FTM3->CNT = 0U;
    FTM3->MOD = 0U;
    FTM3->MODE = 0U;

    // Edge-aligned PWM, high-true pulses.
    FTM3->CONTROLS[2].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;

    // Keep the counter stopped until startUs().
    FTM3->SC = FTM_SC_PS(static_cast<uint32_t>(prescaler_));
}

bool FtmPwm::startUs(uint32_t periodUs) {
    const uint32_t ftmClockHz = getClockHz();

    if (periodUs == 0U || ftmClockHz == 0U) {
        return false;
    }

    // Convert the requested period to timer ticks, rounding up.
    const uint64_t ticks =
        (static_cast<uint64_t>(periodUs) * ftmClockHz + 999'999ULL) / 1'000'000ULL;

    // MOD is 16-bit and represents ticks - 1.
    constexpr uint64_t maxTicks = static_cast<uint64_t>(UINT16_MAX) + 1ULL;

    if (ticks == 0U || ticks > maxTicks) {
        return false;
    }

    // Stop the counter before changing the period.
    FTM3->SC = FTM_SC_PS(static_cast<uint32_t>(prescaler_));

    FTM3->MOD = static_cast<uint16_t>(ticks - 1U);
    FTM3->CNT = 0U;

    // Start the counter using the configured prescaler.
    FTM3->SC = FTM_SC_CLKS(0x01U) | FTM_SC_PS(static_cast<uint32_t>(prescaler_));

    return true;
}

void FtmPwm::setDutyCycle(uint8_t dutyPercent) {
    if (dutyPercent > 100U) {
        dutyPercent = 100U;
    }

    const uint32_t periodTicks = static_cast<uint32_t>(ftm_->MOD) + 1U;

    uint32_t compare = (periodTicks * dutyPercent) / 100U;

    // CnV is 16-bit; cap the compare value at MOD.
    if (compare > ftm_->MOD) {
        compare = ftm_->MOD;
    }

    ftm_->CONTROLS[2].CnV = static_cast<uint16_t>(compare);
}

} // namespace Drivers