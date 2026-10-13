#pragma once

#include <cstddef>
#include <cstdint>

namespace Drivers {

/**
 * Configure a DMA transfer with independent source and destination
 * address stepping and modulo behavior.
 */
template<typename T>
bool Dma::configTransfer(
    const T* src,
    volatile T* dest,
    int16_t sourceStep,
    int16_t destStep,
    uint16_t sourceModuloCount,
    uint16_t destModuloCount,
    uint16_t samplesPerMinorLoop,
    uint16_t minorLoopCount) {

    static_assert(
        sizeof(T) == 1U || sizeof(T) == 2U || sizeof(T) == 4U,
        "Unsupported DMA transfer size");

    // Validate pointers and loop counters before configuring the TCD.
    if (src == nullptr ||
        dest == nullptr ||
        samplesPerMinorLoop == 0U ||
        minorLoopCount == 0U ||
        minorLoopCount > 0x7FFFU) {
        return false;
    }

    // DMA addresses must remain aligned to the transferred type.
    // int32_t makes the signed stride checks explicit.
    const int32_t srcStep = sourceStep;
    const int32_t dstStep = destStep;
    const int32_t elementSize = static_cast<int32_t>(sizeof(T));

    if ((srcStep % elementSize) != 0 ||
        (dstStep % elementSize) != 0) {
        return false;
    }

    // Hardware modulo requires a moving address.
    if ((sourceModuloCount != 0U && sourceStep == 0) ||
        (destModuloCount != 0U && destStep == 0)) {
        return false;
    }

    // Use absolute strides to calculate the address span of each region.
    // int32_t avoids negating an int16_t directly, which could overflow.
    const std::size_t srcStride =
        static_cast<std::size_t>(srcStep < 0 ? -srcStep : srcStep);

    const std::size_t dstStride =
        static_cast<std::size_t>(dstStep < 0 ? -dstStep : dstStep);

    std::size_t sourceModuloBytes = 0U;
    std::size_t destModuloBytes = 0U;

    // A zero count disables modulo; otherwise calculate the region size.
    if (sourceModuloCount != 0U) {
        sourceModuloBytes =
            static_cast<std::size_t>(sourceModuloCount) * srcStride;
    }

    if (destModuloCount != 0U) {
        destModuloBytes =
            static_cast<std::size_t>(destModuloCount) * dstStride;
    }

    // Convert a power-of-two region size to the eDMA SMOD/DMOD encoding.
    // The hardware uses this encoding instead of a byte count.
    const auto getModuloEncoding =
        [](std::size_t bytes, uint8_t& encoding) -> bool {
            if (bytes < 2U ||
                (bytes & (bytes - 1U)) != 0U) {
                return false;
            }

            encoding = 0U;

            while (bytes > 1U) {
                bytes >>= 1U;
                ++encoding;
            }

            return encoding <= 31U;
        };

    uint8_t sourceModulo = 0U;
    uint8_t destModulo = 0U;

    // Validate and encode only the modulo regions that are enabled.
    if (sourceModuloCount != 0U &&
        !getModuloEncoding(sourceModuloBytes, sourceModulo)) {
        return false;
    }

    if (destModuloCount != 0U &&
        !getModuloEncoding(destModuloBytes, destModulo)) {
        return false;
    }

    // Convert pointers to integers to check hardware modulo alignment.
    const std::uintptr_t sourceAddress =
        reinterpret_cast<std::uintptr_t>(src);

    const std::uintptr_t destAddress =
        reinterpret_cast<std::uintptr_t>(dest);

    // The hardware modulo requires base alignment to the region size.
    // Use alignas(2^(x)) to ensure this.
    if ((sourceModuloCount != 0U &&
         (sourceAddress & (sourceModuloBytes - 1U)) != 0U) ||
        (destModuloCount != 0U &&
         (destAddress & (destModuloBytes - 1U)) != 0U)) {
        return false;
    }

    // Disable DMA requests before changing the channel TCD.
    disableHardwareRequests();

    DMA0->TCD[channel_].SADDR =
        static_cast<uint32_t>(sourceAddress);

    DMA0->TCD[channel_].DADDR =
        static_cast<uint32_t>(destAddress);

    // Configure address stepping. A zero step keeps the address fixed.
    DMA0->TCD[channel_].SOFF = sourceStep;
    DMA0->TCD[channel_].DOFF = destStep;

    // Source and destination transfer sizes both match T.
    const uint8_t transferSize = dmaTransferSize<T>();

    DMA0->TCD[channel_].ATTR =
        DMA_ATTR_SSIZE(transferSize) |
        DMA_ATTR_DSIZE(transferSize) |
        DMA_ATTR_SMOD(sourceModulo) |
        DMA_ATTR_DMOD(destModulo);

    // NBYTES is the number of bytes transferred per minor loop.
    DMA0->TCD[channel_].NBYTES_MLNO =
        static_cast<uint32_t>(samplesPerMinorLoop) * sizeof(T);

    // Modulo handles circular address wrapping; no end-of-loop offset needed.
    DMA0->TCD[channel_].SLAST = 0U;
    DMA0->TCD[channel_].DLAST_SGA = 0U;

    // Configure the number of minor loops in each major loop.
    DMA0->TCD[channel_].BITER_ELINKNO = minorLoopCount;
    DMA0->TCD[channel_].CITER_ELINKNO = minorLoopCount;

    // No major-loop interrupt or scatter/gather chaining.
    DMA0->TCD[channel_].CSR = 0U;

    return true;
}


/**
 * Configure a linear transfer.
 */
template<typename T>
bool Dma::configLinearTransfer(
    const T* src,
    volatile T* dest,
    int16_t destStep,
    uint16_t samplesPerMinorLoop,
    uint16_t minorLoopCount) {

    return configTransfer<T>(
        src,
        dest,
        static_cast<int16_t>(sizeof(T)), // Increment source
        destStep,                        // Destination stride
        0U,                              // No source modulo
        0U,                              // No destination modulo
        samplesPerMinorLoop,
        minorLoopCount);
}


/**
 * Configure a transfer with a circular source.
 */
template<typename T>
bool Dma::configCircularTransfer(
    const T* src,
    volatile T* dest,
    int16_t destStep,
    uint16_t sourceModuloCount,
    uint16_t destModuloCount,
    uint16_t samplesPerMinorLoop,
    uint16_t minorLoopCount) {

    if (sourceModuloCount == 0U) {
        return false;
    }

    return configTransfer<T>(
        src,
        dest,
        static_cast<int16_t>(sizeof(T)), // Increment source
        destStep,
        sourceModuloCount,               // Circular source region
        destModuloCount,                 // Optional destination modulo
        samplesPerMinorLoop,
        minorLoopCount);
}

template<typename T>
constexpr uint8_t Dma::dmaTransferSize()
{
    static_assert(
        sizeof(T) == 1 ||
        sizeof(T) == 2 ||
        sizeof(T) == 4,
        "Unsupported DMA transfer size"
    );

    if constexpr (sizeof(T) == 1) {
        return 0U; // 8-bit
    }
    else if constexpr (sizeof(T) == 2) {
        return 1U; // 16-bit
    }
    else {
        return 2U; // 32-bit
    }
}

} // namespace Drivers