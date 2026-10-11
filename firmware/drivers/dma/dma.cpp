#include "dma.hpp"

namespace Drivers {

Dma::Dma(Channel channel) : channel_(static_cast<uint8_t>(channel)) {}

void Dma::init() {
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
}

void Dma::start() {
    // Trigger one DMA service request.
    DMA0->SSRT = channel_;
}

void Dma::enableHardwareRequests() {
    DMA0->SERQ = channel_;
}

void Dma::disableHardwareRequests() {
    DMA0->CERQ = channel_;
}

bool Dma::isComplete() const {
    return (DMA0->TCD[channel_].CSR & DMA_CSR_DONE_MASK) != 0U;
}

bool Dma::hasError() const {
    return (DMA0->ERR & (1UL << channel_)) != 0U;
}

uint16_t Dma::getCurrentIteration() const {
    return static_cast<uint16_t>(DMA0->TCD[channel_].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK);
}

} // namespace Drivers