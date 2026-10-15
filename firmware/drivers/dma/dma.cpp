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

bool Dma::configureMajorLoopLink(Channel target) {
    const uint8_t targetChannel = static_cast<uint8_t>(target);

    // Reject invalid or self-linking targets.
    if (targetChannel >= 16U || targetChannel == channel_) {
        return false;
    }

    // Do not modify a channel while it is active or completed.
    const uint16_t csr = DMA0->TCD[channel_].CSR;

    if ((csr & DMA_CSR_ACTIVE_MASK) != 0U || (csr & DMA_CSR_DONE_MASK) != 0U) {
        return false;
    }

    // Configure the major-loop link to the target channel.
    uint16_t newCsr = csr;
    newCsr &= static_cast<uint16_t>(~DMA_CSR_MAJORLINKCH_MASK);
    newCsr |= DMA_CSR_MAJORLINKCH(targetChannel);
    newCsr |= DMA_CSR_MAJORELINK_MASK;

    DMA0->TCD[channel_].CSR = newCsr;

    return true;
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