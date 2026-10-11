#pragma once

#include "dma.hpp"
#include <cstdint>

/**
 * Routes K22F DMA sources, called slots, to any of the 16 DMA channels
 */
namespace Drivers {

class DmaMux {
  public:
    enum class Source : uint8_t { Disabled = 0U, Dac0 = 45U };

    void init();
    bool configure(Dma::Channel channel, Source source);
    bool disable(Dma::Channel channel);
};

} // namespace Drivers