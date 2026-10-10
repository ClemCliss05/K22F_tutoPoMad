#pragma once

#include <cstdint>

namespace Drivers {

/**
 * Controls the K22F DAC0 peripheral.
 */
class Dac {
  public:
    // Initialize the DAC without the data buffer.
    // Pin DAC0_OUT -> DAC0_OUT
    void init();

    // Initialize the DAC with the buffer in normal mode.
    void initNormalBuffer();

    // Initialize the DAC with the buffer in FIFO mode.
    void initFifo();

    // Write 12-bit value to DAT[0] | DAT[index] | DAT[0] Fifo mode.
    bool write(uint16_t value, uint8_t index = 0U);

    // Return the FIFO data register address for DMA configuration.
    volatile uint16_t *buffAddress();

    // Select software triggering instead of hardware triggering.
    void enableSoftwareTrigger();

    // Request a software-triggered DAC conversion.
    void softwareTrigger();

  private:
    // Maximum value supported by the 12-bit DAC.
    static constexpr uint16_t MaxValue = 4095U;
};

} // namespace Drivers