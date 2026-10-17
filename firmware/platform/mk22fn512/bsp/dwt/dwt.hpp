#pragma once

#include <cstdint>

namespace Bsp {
/**
 * Provides cycle-accurate execution time measurement using the Cortex-M4 DWT.
 */
class Dwt {
  public:
    void init();
    uint32_t getCycles() const;
};

} // namespace Bsp