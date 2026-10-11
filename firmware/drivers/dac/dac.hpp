#pragma once

#include <cstdint>

namespace Drivers {

/**
 * Controls the K22F DAC0 peripheral.
 */
class Dac {
  public:
    // Maximum value supported by the 12-bit DAC.
    static constexpr uint16_t MaxValue = 4095U;
    static constexpr uint8_t BufferSize = 16U;

    /**
     * Buffer event used to trigger the DMA request mechanism.
     */
    enum class BufferEvent : uint8_t { PointerBottom, PointerTop, Watermark };

    // Pin DAC0_OUT -> DAC0_OUT
    void init();
    bool initNormalBuffer(uint8_t watermark = 0U);
    bool initFifo(uint8_t watermark = 0U);

    /**
     * Write a 12-bit value to the DAC.
     * Normal mode:
     *   index selects DAT[index].
     * FIFO mode:
     *   index must be 0; successive writes feed the FIFO.
     */
    bool write(uint16_t value, uint8_t index = 0U);
    // Address of DAT[0], used as the DMA destination.
    volatile uint16_t *buffAddress();

    // Select the DAC buffer event used by the DMA configuration.
    bool configureBufferEvent(BufferEvent event);
    // Enable or disable DAC DMA requests.
    void enableDmaRequests();
    void disableDmaRequests();

    // Select or issue a software-triggered DAC conversion.
    void enableSoftwareTrigger();
    void disableSoftwareTrigger();
    void softwareTrigger();

  private:
    // Tracks the configured buffer mode for write() validation.
    bool fifoMode_{false};
};

} // namespace Drivers