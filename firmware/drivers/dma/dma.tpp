#pragma once

#include <cstddef>
#include <cstdint>

namespace Drivers {

/*
 * Configure either the hardware TCD or a RAM-resident TCD.
 */
template <typename T>
bool Dma::configTransferToTcd(
    Tcd& tcd,
    const T* src,
    volatile T* dest,
    int16_t sourceStep,
    int16_t destStep,
    uint16_t sourceModuloCount,
    uint16_t destModuloCount,
    uint16_t samplesPerMinorLoop,
    uint16_t minorLoopCount,
    bool hardwareTcd) {

    static_assert(
        sizeof(T) == 1U || sizeof(T) == 2U || sizeof(T) == 4U,
        "Unsupported DMA transfer size");

    // Validate parameters before modifying the TCD.
    if (src == nullptr ||
        dest == nullptr ||
        samplesPerMinorLoop == 0U ||
        minorLoopCount == 0U ||
        minorLoopCount > 0x7FFFU) {
        return false;
    }

    // int32_t makes the signed stride checks explicit.
    const int32_t srcStep = sourceStep;
    const int32_t dstStep = destStep;
    const int32_t elementSize = static_cast<int32_t>(sizeof(T));

    // Strides must preserve the alignment of the transferred type.
    if ((srcStep % elementSize) != 0 ||
        (dstStep % elementSize) != 0) {
        return false;
    }

    // Modulo requires an address that changes between transfers.
    if ((sourceModuloCount != 0U && sourceStep == 0) ||
        (destModuloCount != 0U && destStep == 0)) {
        return false;
    }

    // Use absolute offsets to calculate the address range for each region.
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

    // Convert a power-of-two modulo region to the hardware encoding.
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

    // Modulo regions must start at addresses aligned to their size.
    // Use alignas(2^(x)) to ensure this.
    if ((sourceModuloCount != 0U &&
         (sourceAddress & (sourceModuloBytes - 1U)) != 0U) ||
        (destModuloCount != 0U &&
         (destAddress & (destModuloBytes - 1U)) != 0U)) {
        return false;
    }

    // Disable hardware requests only after validation succeeds.
    if (hardwareTcd) {
        disableHardwareRequests();
    }

    tcd.SADDR = static_cast<uint32_t>(sourceAddress);
    tcd.DADDR = static_cast<uint32_t>(destAddress);

    // Configure address stepping. A zero step keeps the address fixed.
    tcd.SOFF = sourceStep;
    tcd.DOFF = destStep;

    // Source and destination transfer sizes both match T.
    const uint8_t transferSize = dmaTransferSize<T>();

    tcd.ATTR =
        DMA_ATTR_SSIZE(transferSize) |
        DMA_ATTR_DSIZE(transferSize) |
        DMA_ATTR_SMOD(sourceModulo) |
        DMA_ATTR_DMOD(destModulo);

    // NBYTES is the number of bytes transferred per minor loop.
    tcd.NBYTES_MLNO =
        static_cast<uint32_t>(samplesPerMinorLoop) * sizeof(T);

    // Modulo handles wrapping; no end-of-loop offset is required.
    tcd.SLAST = 0U;
    tcd.DLAST_SGA = 0U;

    // Configure the number of minor loops in each major loop.
    tcd.BITER_ELINKNO = minorLoopCount;
    tcd.CITER_ELINKNO = minorLoopCount;

    // Clear previous channel options and scatter-gather settings.
    tcd.CSR = 0U;

    return true;
}

template <typename T>
bool Dma::configureScatterGatherDescriptor(
    ScatterGatherDescriptor& descriptor,
    const T* src,
    volatile T* dest,
    int16_t sourceStep,
    int16_t destStep,
    uint16_t sourceModuloCount,
    uint16_t destModuloCount,
    uint16_t samplesPerMinorLoop,
    uint16_t minorLoopCount) {

    return configTransferToTcd(
        descriptor.tcd_,
        src,
        dest,
        sourceStep,
        destStep,
        sourceModuloCount,
        destModuloCount,
        samplesPerMinorLoop,
        minorLoopCount,
        false);
}

/**
 * Configure a DMA transfer with independent source and destination
 * address stepping and modulo behavior.
 */
template <typename T>
bool Dma::configTransfer(
    const T* src,
    volatile T* dest,
    int16_t sourceStep,
    int16_t destStep,
    uint16_t sourceModuloCount,
    uint16_t destModuloCount,
    uint16_t samplesPerMinorLoop,
    uint16_t minorLoopCount) {

    return configTransferToTcd(
        DMA0->TCD[channel_],
        src,
        dest,
        sourceStep,
        destStep,
        sourceModuloCount,
        destModuloCount,
        samplesPerMinorLoop,
        minorLoopCount,
        true);
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