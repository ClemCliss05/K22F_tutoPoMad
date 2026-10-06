#pragma once

#include <cstdint>

namespace Drivers {

class Pdb {
  public:
    explicit Pdb(uint32_t busClockHz);

    void init();
    bool start(uint32_t periodUs);

    void stop();

    uint32_t getClockHz() const;

  private:
    uint32_t busClockHz_;
};

} // namespace Drivers