#include "dmamux.hpp"

namespace Drivers {

void DmaMux::init() {
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
}

bool DmaMux::configure(Dma::Channel channel, Source source) {
    const uint8_t index = static_cast<uint8_t>(channel);
    const uint8_t sourceValue = static_cast<uint8_t>(source);

    if (index >= 16U || sourceValue > 63U) {
        return false;
    }

    DMAMUX->CHCFG[index] = 0U;

    DMAMUX->CHCFG[index] = DMAMUX_CHCFG_SOURCE(sourceValue) | DMAMUX_CHCFG_ENBL_MASK;

    return true;
}

bool DmaMux::disable(Dma::Channel channel) {
    const uint8_t index = static_cast<uint8_t>(channel);

    if (index >= 16U) {
        return false;
    }

    DMAMUX->CHCFG[index] = 0U;
    return true;
}

} // namespace Drivers