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
    void init();
    void write(uint16_t value);

    void initFIFO();
    void enableBuffer();
    void writeBuffer(uint16_t value);

    void trigger();

  private:
    // 12-bit DAC
    static constexpr uint16_t MaxValue = 4095U;
};
} // namespace Drivers