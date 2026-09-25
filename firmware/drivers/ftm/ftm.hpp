#pragma once

#include <cstdint>

namespace Drivers {

class Ftm {
  public:
    /*
     * FTM driver functions
     * PTB18 -> FTM2_CH0
     */

    explicit Ftm(uint32_t clockHz);

    void init();

    uint32_t getClockHz() const;

  private:
    uint32_t clockHz_;
};

} // namespace Drivers