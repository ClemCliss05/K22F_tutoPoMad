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
        Channel0 = 0,
        Channel1 = 1,
        Channel2 = 2,
        Channel3 = 3,
        Channel4 = 4,
        Channel5 = 5,
        Channel6 = 6,
        Channel7 = 7,
        Channel8 = 8,
        Channel9 = 9,
        Channel10 = 10,
        Channel11 = 11,
        Channel12 = 12,
        Channel13 = 13,
        Channel14 = 14,
        Channel15 = 15,
    };

    explicit Dma(Channel channel);

    // Configure a memory-to-memory transfer with the variable type T.
    template <typename T>
    void configureMemoryToMemory(const T *source, T *destination, uint16_t elementCount);

    // Configure a memory-to-peripheral transfer with the variable type T.
    template <typename T>
    void configureMemoryToPeripheral(const T *source, volatile T *destination,
                                     uint16_t elementCount);

    // Trigger one DMA service request.
    void start();

    // Check if the current transfer is complete.
    bool isComplete() const;

    // Check if a DMA error occurred.
    bool hasError() const;

    // Return the number of remaining minor loops.
    uint16_t getCurrentIteration() const;

  private:
    uint8_t channel_;

    // Return the DMA transfer-size encoding for the given C++ type.
    template <typename T> static constexpr uint8_t dmaTransferSize();
};

} // namespace Drivers

// Template implementations must be visible when the compiler instantiates them.
#include "dma.tpp"