#pragma once

#include <cstdint>

#include "pit.hpp"

namespace Services {

class Delay {
  public:
    explicit Delay(Drivers::Pit &pit);

    /**
     * Blocking delay in milliseconds.
     *
     * Example:
     *   delay.ms(100); // wait approximately 100 ms
     */
    void ms(uint32_t milliseconds);

    /**
     * Blocking delay in microseconds.
     *
     * Example:
     *   delay.us(100); // wait approximately 100 us
     */
    void us(uint32_t microseconds);

  private:
    Drivers::Pit &pit_;
};

} // namespace Services