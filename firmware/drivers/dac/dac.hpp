#pragma once

#include <cstdint>

namespace Drivers {
/**
 * Configures and controls the K22F DAC output.
 */
class Dac {
  public:
    /*
     * Pin DAC0_OUT -> DAC0_OUT
     */
    void init(void);
    void write(uint16_t value);

  private:
};
} // namespace Drivers