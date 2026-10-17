#pragma once

#include <cstdint>

namespace Drivers {
/**
 * Configures and reads the K22F ADC channels.
 */
class Adc {
  public:
    // Pin PTB0 -> ADC0_SE8
    void init(void);
    uint16_t read(void);
    uint8_t toPercent(uint16_t adcValue) const;

  private:
};
} // namespace Drivers