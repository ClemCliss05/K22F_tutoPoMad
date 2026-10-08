#include "dma.hpp"

namespace Drivers {

Dma::Dma(Channel channel)
    : channel_(static_cast<uint8_t>(channel))
{
}

void Dma::start()
{
    // Trigger one DMA service request.
    DMA0->SSRT = channel_;
}

bool Dma::isComplete() const
{
    return (DMA0->TCD[channel_].CSR & DMA_CSR_DONE_MASK) != 0U;
}

bool Dma::hasError() const
{
    return (DMA0->ERR & (1UL << channel_)) != 0U;
}

uint16_t Dma::getCurrentIteration() const
{
    return DMA0->TCD[channel_].CITER_ELINKNO &
           DMA_CITER_ELINKNO_CITER_MASK;
}

} // namespace Drivers