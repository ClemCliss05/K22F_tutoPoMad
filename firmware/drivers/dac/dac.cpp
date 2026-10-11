
#include "dac.hpp"
#include "MK22FN512.h"

namespace Drivers {

void Dac::init() {
    // Enable the DAC0 peripheral clock.
    SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;

    // Disable the DAC before changing its configuration.
    DAC0->C0 = 0U;

    // Reset buffer configuration and upper-limit settings.
    DAC0->C1 = 0U;
    DAC0->C2 = 0U;

    // Reset the software state to normal buffer mode.
    fifoMode_ = false;

    // Select VDDA as the DAC reference and enable the peripheral.
    DAC0->C0 = DAC_C0_DACRFS_MASK | DAC_C0_DACEN_MASK;
}

bool Dac::initNormalBuffer(uint8_t watermark) {
    // The watermark field accepts values from 0 to 3.
    if (watermark > 3U) {
        return false;
    }

    // Initialize the common DAC configuration.
    init();

    // Enable the buffer in normal mode.
    // DMA requests remain disabled until explicitly enabled.
    DAC0->C1 = DAC_C1_DACBFEN_MASK | DAC_C1_DACBFMD(0U) | DAC_C1_DACBFWM(watermark);

    // Configure all 16 buffer entries: DAT[0] through DAT[15].
    DAC0->C2 = DAC_C2_DACBFUP(15U);

    return true;
}

bool Dac::initFifo(uint8_t watermark) {
    // The watermark field accepts values from 0 to 3.
    if (watermark > 3U) {
        return false;
    }

    // Initialize the common DAC configuration.
    init();

    // Configure the DAC buffer in FIFO mode.
    // The watermark determines when the FIFO event is asserted.
    // DMA requests remain disabled until explicitly enabled.
    DAC0->C1 = DAC_C1_DACBFEN_MASK | DAC_C1_DACBFMD(3U) | DAC_C1_DACBFWM(watermark);

    // Configure the FIFO buffer upper limit.
    DAC0->C2 = DAC_C2_DACBFUP(15U);

    // Remember that writes must feed the FIFO through DAT[0].
    fifoMode_ = true;

    return true;
}

bool Dac::write(uint16_t value, uint8_t index) {
    // In FIFO mode, successive writes feed the FIFO through DAT[0].
    // An indexed write would not represent the intended FIFO operation.
    if (fifoMode_ && index != 0U) {
        return false;
    }

    // In normal mode, make sure the selected buffer entry exists.
    if (index >= BufferSize) {
        return false;
    }

    // Keep the value within the 12-bit DAC range.
    value &= MaxValue;

    // Write the low 8 bits, then the upper 4 bits.
    DAC0->DAT[index].DATL = static_cast<uint8_t>(value);
    DAC0->DAT[index].DATH = static_cast<uint8_t>(value >> 8U);

    // With FIFO mode enabled, index must be zero, so this write
    // feeds the FIFO through DAT[0].
    return true;
}

volatile uint16_t *Dac::buffAddress() {
    // Return the address of DAT[0].
    // Normal buffer DMA:
    //   Configure the DMA destination to advance through DAT[0..15].
    // FIFO DMA:
    //   Keep the DMA destination fixed at DAT[0] so each write
    //   pushes the next sample into the FIFO.
    return reinterpret_cast<volatile uint16_t *>(&DAC0->DAT[0].DATL);
}

bool Dac::configureBufferEvent(BufferEvent event) {
    uint8_t eventMask = 0U;

    // Convert the requested event to its corresponding C0 bit.
    switch (event) {
    case BufferEvent::PointerBottom:
        eventMask = DAC_C0_DACBBIEN_MASK;
        break;

    case BufferEvent::PointerTop:
        eventMask = DAC_C0_DACBTIEN_MASK;
        break;

    case BufferEvent::Watermark:
        eventMask = DAC_C0_DACBWIEN_MASK;
        break;

    default:
        return false;
    }

    // Disable the other buffer events before selecting one.
    constexpr uint8_t allEventMasks =
        DAC_C0_DACBBIEN_MASK | DAC_C0_DACBTIEN_MASK | DAC_C0_DACBWIEN_MASK;

    DAC0->C0 &= static_cast<uint8_t>(~allEventMasks);
    DAC0->C0 |= eventMask;

    return true;
}

void Dac::enableDmaRequests() {
    // Enable the DAC DMA request mechanism.
    // Select the buffer event separately with configureBufferEvent().
    DAC0->C1 |= DAC_C1_DMAEN_MASK;
}

void Dac::disableDmaRequests() {
    // Disable DMA requests first.
    DAC0->C1 &= static_cast<uint8_t>(~DAC_C1_DMAEN_MASK);

    // Then disable all buffer event enables.
    constexpr uint8_t allEventMasks =
        DAC_C0_DACBBIEN_MASK | DAC_C0_DACBTIEN_MASK | DAC_C0_DACBWIEN_MASK;

    DAC0->C0 &= static_cast<uint8_t>(~allEventMasks);
}

void Dac::enableSoftwareTrigger() {
    // Select software triggering instead of hardware triggering.
    DAC0->C0 |= DAC_C0_DACTRGSEL_MASK;
}

void Dac::disableSoftwareTrigger() {
    // Clear the selection bit to use hardware triggering.
    DAC0->C0 &= static_cast<uint8_t>(~DAC_C0_DACTRGSEL_MASK);
}

void Dac::softwareTrigger() {
    // Issue a software trigger.
    DAC0->C0 |= DAC_C0_DACSWTRG_MASK;
}

} // namespace Drivers