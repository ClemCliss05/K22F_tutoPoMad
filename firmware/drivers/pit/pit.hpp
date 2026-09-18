#pragma once

#include <cstdint>

namespace Drivers {

class Pit {
  public:
    explicit Pit(uint32_t clockHz);

    void init();

    /**
     * Start the PIT as a periodic 1 ms system tick.
     *
     * The PIT generates one interrupt every millisecond.
     */
    bool start();

    void stop();

    /**
     * Return the number of elapsed PIT ticks since initialization.
     *
     * One tick corresponds to 1 ms.
     */
    uint32_t getTicks() const;

    uint32_t getClockHz() const;

  private:
    uint32_t clockHz_;
};

} // namespace Drivers