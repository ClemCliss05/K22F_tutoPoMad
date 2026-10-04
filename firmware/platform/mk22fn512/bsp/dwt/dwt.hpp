#pragma once

#include <cstdint>

namespace Bsp {

class Dwt {
  public:
    void init();
    uint32_t getCycles() const;
};

} // namespace Bsp