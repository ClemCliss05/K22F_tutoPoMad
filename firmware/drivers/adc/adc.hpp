#pragma once

#include <cstdint>

namespace Drivers {
class Adc {
  public:
    /*
     * ADC driver functions
     * PTB0 -> ADC0_SE8
     */

    void init(void);
    uint16_t read(void);
    uint8_t toPercent(uint16_t adcValue);

  private:
};
} // namespace Drivers