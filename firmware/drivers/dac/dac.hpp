#pragma once

#include <cstdint>

namespace Drivers {
class Dac {
  public:
    /*
     * DAC driver functions
     * DAC0_OUT -> DAC0_OUT
     */

    void init(void);
    void write(uint16_t value);

  private:
};
} // namespace Drivers