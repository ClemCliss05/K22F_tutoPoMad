#include "ftm.hpp"
#include "interrupt.hpp"

namespace Drivers
{
    // ============================================================
    // Ftm
    // ============================================================

    Ftm::Ftm(
        FTM_Type* ftm,
        uint32_t busClockHz
    )
        : ftm_(ftm),
          busClockHz_(busClockHz)
    {
    }


    uint32_t Ftm::getClockHz() const
    {
        const uint32_t prescaler =
            ((ftm_->SC & FTM_SC_PS_MASK) >> FTM_SC_PS_SHIFT);

        return busClockHz_ / (1U << prescaler);
    }


    uint32_t Ftm::ticksToMs(uint32_t ticks) const
    {
        const uint32_t ftmClockHz = getClockHz();

        return static_cast<uint32_t>(
            (static_cast<uint64_t>(ticks) * 1000ULL)
            / ftmClockHz
        );
    }


    // ============================================================
    // FtmInCap
    // ============================================================

    FtmInCap::FtmInCap(
        FTM_Type* ftm,
        uint32_t busClockHz
    )
        : Ftm(ftm, busClockHz)
    {
    }


    void FtmInCap::init()
    {
        // Peripheral clocks
        SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
        SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

        // PTB18 -> FTM2_CH0
        PORTB->PCR[18] &= ~PORT_PCR_MUX_MASK;
        PORTB->PCR[18] |= PORT_PCR_MUX(0x03);

        // Reset FTM configuration
        ftm_->SC = 0U;
        ftm_->CONTROLS[0].CnSC = 0U;
        ftm_->CONTROLS[0].CnV = 0U;

        ftm_->CNTIN = 0U;
        ftm_->CNT = 0U;
        ftm_->MODE = 0U;
        ftm_->MOD = 0xFFFFU;

        // Input Capture:
        // rising + falling edge
        ftm_->CONTROLS[0].CnSC =
            FTM_CnSC_ELSA_MASK |
            FTM_CnSC_ELSB_MASK;

        // FTM clock = bus clock / 16
        ftm_->SC =
            FTM_SC_CLKS(0x01) |
            FTM_SC_PS(0x04);

        // Enable interrupts
        ftm_->CONTROLS[0].CnSC |= FTM_CnSC_CHIE_MASK;
        ftm_->SC |= FTM_SC_TOIE_MASK;

        NVIC_ClearPendingIRQ(FTM2_IRQn);
        NVIC_EnableIRQ(FTM2_IRQn);
    }


    bool FtmInCap::captureAvailable() const
    {
        return Interrupt::ftm2CapturePending;
    }


    bool FtmInCap::readCapture(Capture& capture)
    {
        if (!Interrupt::ftm2CapturePending)
        {
            return false;
        }

        NVIC_DisableIRQ(FTM2_IRQn);

        capture.timestamp =
            Interrupt::ftm2CaptureTime;

        capture.level =
            Interrupt::ftm2CaptureLevel;

        Interrupt::ftm2CapturePending = false;

        NVIC_EnableIRQ(FTM2_IRQn);

        return true;
    }


    // ============================================================
    // FtmPwm
    // ============================================================

    FtmPwm::FtmPwm(
        FTM_Type* ftm,
        uint32_t busClockHz
    )
        : Ftm(ftm, busClockHz)
    {
    }


    void FtmPwm::init()
    {
        // PWM implementation coming next.
    }


    void FtmPwm::setDutyCycle(uint8_t percent)
    {
        // PWM implementation coming next.
        (void)percent;
    }
}
