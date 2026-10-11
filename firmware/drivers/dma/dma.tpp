#pragma once

namespace Drivers {


template<typename T>
bool Dma::configLinearTransfer(
    const T* src,
    volatile T* dest,
    int16_t destStep,
    uint16_t samplesPerMinorLoop,
    uint16_t minorLoopCount) {

    static_assert(
        sizeof(T) == 1U || sizeof(T) == 2U || sizeof(T) == 4U,
        "Unsupported DMA transfer size");

    if (src == nullptr || dest == nullptr ||
        samplesPerMinorLoop == 0U ||
        minorLoopCount == 0U ||
        minorLoopCount > 0x7FFFU) {
        return false;
    }

    // Disable requests before updating the TCD.
    disableHardwareRequests();

    DMA0->TCD[channel_].SADDR =
        static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(src));

    DMA0->TCD[channel_].DADDR =
        static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(dest));

    // Advance the source by one element per transfer.
    DMA0->TCD[channel_].SOFF =
        static_cast<int16_t>(sizeof(T));

    // Apply the destination stride after each transfer.
    DMA0->TCD[channel_].DOFF = destStep;

    // Source and destination transfer sizes match T.
    const uint8_t transferSize = dmaTransferSize<T>();

    DMA0->TCD[channel_].ATTR =
        DMA_ATTR_SSIZE(transferSize) |
        DMA_ATTR_DSIZE(transferSize);

    // Transfer samplesPerMinorLoop elements per minor loop.
    DMA0->TCD[channel_].NBYTES_MLNO =
        static_cast<uint32_t>(samplesPerMinorLoop) * sizeof(T);

    // Do not adjust addresses at major-loop completion.
    DMA0->TCD[channel_].SLAST = 0U;
    DMA0->TCD[channel_].DLAST_SGA = 0U;

    // Number of minor loops in one major loop.
    DMA0->TCD[channel_].BITER_ELINKNO = minorLoopCount;
    DMA0->TCD[channel_].CITER_ELINKNO = minorLoopCount;

    // No interrupt or scatter/gather chaining.
    DMA0->TCD[channel_].CSR = 0U;

    return true;
}

template<typename T>
bool Dma::configCircularTransfer(
    const T* sourceBase,
    uint16_t sourceCount,
    volatile T* dest,
    int16_t destStep,
    uint16_t destModuloCount,
    uint16_t samplesPerMinorLoop,
    uint16_t minorLoopCount) {

    static_assert(
        sizeof(T) == 1U || sizeof(T) == 2U || sizeof(T) == 4U,
        "Unsupported DMA transfer size");

    // Validate the transfer parameters.
    if (sourceBase == nullptr || dest == nullptr ||
        sourceCount == 0U ||
        destModuloCount == 0U ||
        samplesPerMinorLoop == 0U ||
        minorLoopCount == 0U ||
        minorLoopCount > 0x7FFFU ||
        destStep < static_cast<int16_t>(sizeof(T)) ||
        (destStep % static_cast<int16_t>(sizeof(T))) != 0) {
        return false;
    }

    const std::size_t sourceBytes =
        static_cast<std::size_t>(sourceCount) * sizeof(T);

    // The destination modulo region includes the address stride.
    const std::size_t destModuloBytes =
        static_cast<std::size_t>(destModuloCount) *
        static_cast<std::size_t>(destStep);

    // Encode a power-of-two modulo region, at least 2 bytes.
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

    if (!getModuloEncoding(sourceBytes, sourceModulo) ||
        !getModuloEncoding(destModuloBytes, destModulo)) {
        return false;
    }

    const std::uintptr_t sourceAddress =
        reinterpret_cast<std::uintptr_t>(sourceBase);

    const std::uintptr_t destAddress =
        reinterpret_cast<std::uintptr_t>(dest);

    // Modulo regions must be aligned to their size.
    // use alignas(2^(x)) to ensure this
    if ((sourceAddress & (sourceBytes - 1U)) != 0U ||
        (destAddress & (destModuloBytes - 1U)) != 0U) {
        return false;
    }

    // Disable requests before updating the TCD.
    disableHardwareRequests();

    DMA0->TCD[channel_].SADDR =
        static_cast<uint32_t>(
            reinterpret_cast<std::uintptr_t>(
                sourceBase));

    DMA0->TCD[channel_].DADDR =
        static_cast<uint32_t>(destAddress);

    // Advance the source by one element per transfer.
    DMA0->TCD[channel_].SOFF =
        static_cast<int16_t>(sizeof(T));

    // Apply the destination stride after each transfer.
    DMA0->TCD[channel_].DOFF = destStep;

    const uint8_t transferSize = dmaTransferSize<T>();

    DMA0->TCD[channel_].ATTR =
        DMA_ATTR_SSIZE(transferSize) |
        DMA_ATTR_DSIZE(transferSize) |
        DMA_ATTR_SMOD(sourceModulo) |
        DMA_ATTR_DMOD(destModulo);

    // Transfer samplesPerMinorLoop elements per minor loop.
    DMA0->TCD[channel_].NBYTES_MLNO =
        static_cast<uint32_t>(samplesPerMinorLoop) * sizeof(T);

    // Hardware modulo handles address wraparound.
    DMA0->TCD[channel_].SLAST = 0U;
    DMA0->TCD[channel_].DLAST_SGA = 0U;

    // Number of minor loops in one major loop.
    DMA0->TCD[channel_].BITER_ELINKNO = minorLoopCount;
    DMA0->TCD[channel_].CITER_ELINKNO = minorLoopCount;

    // No interrupt or scatter/gather chaining.
    DMA0->TCD[channel_].CSR = 0U;

    return true;
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