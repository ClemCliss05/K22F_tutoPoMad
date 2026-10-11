#pragma once

#include <cstdint>

#include "MK22FN512.h"

namespace Drivers {

/**
 * Configures K22F DMA channels for memory and peripheral data transfers.
 */
class Dma {
  public:
    enum class Channel : uint8_t {
        Channel0 = 0U,
        Channel1 = 1U,
        Channel2 = 2U,
        Channel3 = 3U,
        Channel4 = 4U,
        Channel5 = 5U,
        Channel6 = 6U,
        Channel7 = 7U,
        Channel8 = 8U,
        Channel9 = 9U,
        Channel10 = 10U,
        Channel11 = 11U,
        Channel12 = 12U,
        Channel13 = 13U,
        Channel14 = 14U,
        Channel15 = 15U
    };

    explicit Dma(Channel channel);

    void init();

    // Configure a linear transfer.
    template <typename T>
    bool configLinearTransfer(const T *src, volatile T *dest, int16_t destStep,
                              uint16_t samplesPerMinorLoop, uint16_t minorLoopCount);

    // Configure a circular source and destination transfer.
    template <typename T>
    bool configCircularTransfer(const T *sourceBase, uint16_t sourceCount, volatile T *dest,
                                int16_t destStep, uint16_t destModuloCount,
                                uint16_t samplesPerMinorLoop, uint16_t minorLoopCount);

    // Trigger one software DMA service request.
    void start();

    // Enable or disable hardware DMA requests.
    void enableHardwareRequests();
    void disableHardwareRequests();

    // Return transfer status.
    bool isComplete() const;
    bool hasError() const;
    uint16_t getCurrentIteration() const;

  private:
    uint8_t channel_;

    template <typename T> static constexpr uint8_t dmaTransferSize();
};

} // namespace Drivers

// Template implementations must be visible when the compiler instantiates them.
#include "dma.tpp"