// | State | C1.CLKS | C6.PLLS | S.CLKST | S.IREFST | S.PLLST | S.LOCK0 |
// | ----- | ------- | ------- | ------- | -------- | ------- | ------- |
// | FEI   | 00      | 0       | 00      | 1        | 0       | 0       |
// | FBE   | 10      | 0       | 10      | 0        | 0       | 0       |
// | PBE   | 10      | 1       | 10      | 0        | 1       | 1       |
// | PEE   | 00      | 1       | 11      | 0        | 1       | 1       |

// // 120 MHz PLL Engaged External (PEE) mode steps
// // initOSC()
// // FEI State: FLL Engaged Internal
// MCG->C2->RANGE=01 // High frequency range selected for the crystal oscillator
// MCG->C2->EREFS0=1 // Oscillator requested.
// MCG->C2->HGO0=0 // Configure crystal oscillator for low-power operation
// MCG->C1->CLKS=10 // External reference clock is selected -> transition FEI to FBE
// MCG->C1->IREFS=0 // External reference clock is selected
// MCG->S->OSCINIT0=1 // permits to be sure OSC Initialization is done => 8MHz crystal is stable
// MCG->S->IREFST=0 // permits to be sure Source of FLL reference clock is the external reference clock
// MCG->S->CLKST=10 // permits to be sure External reference clock is selected
// // FBE State: FLL Bypassed External (8MHz)
// // setXXXXHz()
// // DIVIDE THE CLOCK BEFORE TO ENABLE PLL (DON'T FORGET FREQUENCY LIMITATIONS)
// SIM->CLKDIV1->OUTDIV1=0000 // 0000 => divide by 1, core clock = 120MHz
// SIM->CLKDIV1->OUTDIV2=0001 // 0001 => divide by 2, bus clock = 60MHz
// SIM->CLKDIV1->OUTDIV3=0001 // 0001 => divide by 2, flexbus clock = 60MHz
// SIM->CLKDIV1->OUTDIV4=0100 // 0100 => divide by 5, flash clock = 24MHz
// MCG->C5->PRDIV0=00010 // 00010 => 3, 8/3= 2.6666666667MHz
// MCG->C6->VDIV0=10101 // 10101 => 45, 45 * 2.6666666667MHz = 120MHz, see SystemCoreClockUpdate (void)
// MCG->C6->PLLS=1 // activate PLL -> transition FBE to PBE
// MCG->S->PLLST=1 // permits to be sure Source of PLLS clock is PLL output clock.
// MCG->S->LOCK0=1 // permits to be sure PLL is locked
// // PBE State: PLL Bypassed External
// MCG->C1->CLKS=00 // Output of FLL or PLL is selected -> transition PBE to PEE
// MCG->S->CLKST=11 // permits to be sure PLL is selected
// // PEE State: PLL Engaged External

#include "clock.hpp"

#include "system_MK22F51212.h"
#include "MK22FN512.h"

// Init oscillator with external crystal at 8MHz
bool Platform::Clock::initOSC()
{
    uint32_t timeout = 1000000;

    // FEI -> FBE
    MCG->C2 &= ~(MCG_C2_RANGE_MASK |
                 MCG_C2_EREFS0_MASK | 
                 MCG_C2_HGO_MASK);
    MCG->C2 |= MCG_C2_RANGE(0b01) |
               MCG_C2_EREFS0_MASK | 
               MCG_C2_HGO(0b0);
    MCG->C1 &= ~(MCG_C1_CLKS_MASK | 
                 MCG_C1_IREFS_MASK);      
    MCG->C1 |= MCG_C1_CLKS(0b10) | 
               MCG_C1_IREFS(0b0);

    // Wait OSC Initialization is done and External reference clock is selected
	do
    {
        // All conditions must be true to be sure that we are in FBE mode
        if ((MCG->S & MCG_S_OSCINIT0_MASK) == MCG_S_OSCINIT0(0b1) && 
            !(MCG->S & MCG_S_IREFST_MASK) && 
            (MCG->S & MCG_S_CLKST_MASK) == MCG_S_CLKST(0b10))
        {
            SystemCoreClockUpdate();
            return true;
        }
    } while (timeout-- > 0);

    return false;
}

// Set PLL to 120MHz
bool Platform::Clock::set120MHz()
{
    uint32_t timeout = 1000000;
    bool PBE_status = false;

    // DIVIDE THE CLOCK BEFORE TO ENABLE PLL (DON'T FORGET FREQUENCY LIMITATIONS)
    SIM->CLKDIV1 &= ~(SIM_CLKDIV1_OUTDIV1_MASK | 
                      SIM_CLKDIV1_OUTDIV2_MASK | 
                      SIM_CLKDIV1_OUTDIV3_MASK | 
                      SIM_CLKDIV1_OUTDIV4_MASK);
    SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0b0000) |
                    SIM_CLKDIV1_OUTDIV2(0b0001) | 
                    SIM_CLKDIV1_OUTDIV3(0b0001) | 
                    SIM_CLKDIV1_OUTDIV4(0b0100);

    // FBE -> PBE
    MCG->C5 &= ~MCG_C5_PRDIV0_MASK;
    MCG->C5 |= MCG_C5_PRDIV0(0b00010); // 8MHz / 3 = 2.6666666667MHz
    MCG->C6 &= ~(MCG_C6_VDIV0_MASK | 
                 MCG_C6_PLLS_MASK);
    MCG->C6 |= MCG_C6_VDIV0(10101) | // (45) * 2.6666666667MHz = 120MHz
               MCG_C6_PLLS_MASK; // activate PLL

    // Wait Source of PLLS clock is PLL output clock and PLL is locked
	do
    {
        // All conditions must be true to be sure that we are in PBE mode
        if ((MCG->S & MCG_S_PLLST_MASK) && 
            (MCG->S & MCG_S_LOCK0_MASK))
        {
            PBE_status = true;
            break;
        }
    } while ((timeout-- > 0) && !PBE_status);

    if (!PBE_status)
    {
        return false;
    }

    // PBE -> PEE
    MCG->C1 &= ~MCG_C1_CLKS_MASK; // Output of FLL or PLL is selected

    // Wait PLL is selected
    do
    {
        if ((MCG->S & MCG_S_CLKST_MASK) == MCG_S_CLKST(0b11))
        {
            SystemCoreClockUpdate();
            return true;
        }
    } while (timeout-- > 0);

    return false;
}

uint32_t Platform::Clock::getCoreClock()
{
    return SystemCoreClock;
}