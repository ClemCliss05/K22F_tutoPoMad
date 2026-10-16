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
    if ((DMA0->TCD[channel_].CSR & DMA_CSR_ACTIVE_MASK) != 0U ||
        (DMA0->TCD[channel_].CSR & DMA_CSR_DONE_MASK) != 0U) {
        return false;
    }

    // Configure the major-loop link to the target channel.
    DMA0->TCD[channel_].CSR &= static_cast<uint16_t>(~DMA_CSR_MAJORLINKCH_MASK);
    DMA0->TCD[channel_].CSR |= DMA_CSR_MAJORLINKCH(targetChannel);
    DMA0->TCD[channel_].CSR |= DMA_CSR_MAJORELINK_MASK;

    return true;
}

bool Dma::configureMinorLoopLink(Channel target) {
    const uint8_t targetChannel = static_cast<uint8_t>(target);

    // Reject invalid or self-linking targets.
    if (targetChannel >= 16U || targetChannel == channel_) {
        return false;
    }

    auto &tcd = DMA0->TCD[channel_];

    // Do not modify a channel while it is active or completed.
    if ((tcd.CSR & DMA_CSR_ACTIVE_MASK) != 0U || (tcd.CSR & DMA_CSR_DONE_MASK) != 0U) {
        return false;
    }

    const uint16_t citer = tcd.CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK;
    const uint16_t biter = tcd.BITER_ELINKNO & DMA_BITER_ELINKNO_BITER_MASK;

    // ELINKYES uses a 9-bit iteration counter.
    if (citer == 0U || biter == 0U || citer > 0x1FFU || biter > 0x1FFU) {
        return false;
    }

    tcd.CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK_MASK | DMA_CITER_ELINKYES_LINKCH(targetChannel) |
                         DMA_CITER_ELINKYES_CITER(citer);

    tcd.BITER_ELINKYES = DMA_BITER_ELINKYES_ELINK_MASK | DMA_BITER_ELINKYES_LINKCH(targetChannel) |
                         DMA_BITER_ELINKYES_BITER(biter);

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