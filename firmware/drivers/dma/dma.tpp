#pragma once

namespace Drivers {

template<typename T>
    void Dma::configTransfer(
        const T* src,
        volatile T* dest,
        int16_t destStep,
        uint16_t elementCount) {
    DMA0->TCD[channel_].SADDR =
        reinterpret_cast<uint32_t>(src);

    DMA0->TCD[channel_].DADDR =
        reinterpret_cast<uint32_t>(dest);

    // Move one element after each src access.
    DMA0->TCD[channel_].SOFF = sizeof(T);

    // Move one element of destStep offset after each src access.
    DMA0->TCD[channel_].DOFF = destStep;

    // Src and dest accesses match the element type.
    const uint8_t transferSize = dmaTransferSize<T>();

    DMA0->TCD[channel_].ATTR =
        DMA_ATTR_SSIZE(transferSize) |
        DMA_ATTR_DSIZE(transferSize);

    // Transfer one element per minor loop.
    DMA0->TCD[channel_].NBYTES_MLNO = sizeof(T);

    // Number of minor loops in the major loop.
    DMA0->TCD[channel_].BITER_ELINKNO = elementCount;

    // Number of minor loops remaining.
    DMA0->TCD[channel_].CITER_ELINKNO = elementCount;

    // Do not adjust the source or destination at major-loop completion.
    DMA0->TCD[channel_].SLAST = 0;
    DMA0->TCD[channel_].DLAST_SGA = 0;

    // Basic transfer: no interrupt or advanced features.
    DMA0->TCD[channel_].CSR = 0;
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