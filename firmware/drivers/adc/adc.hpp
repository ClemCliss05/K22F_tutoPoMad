#pragma once

#include <cstdint>

namespace Drivers {
class Adc {
    public:

        void init(void);
        uint16_t read(void);
    private:
};
} // namespace Drivers