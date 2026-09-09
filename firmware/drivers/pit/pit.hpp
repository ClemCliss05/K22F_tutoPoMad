#pragma once

#include <cstdint>

namespace Drivers {
    class Pit {
        public:
            
            // FOR THE MOMENT ONLY WORKING FOR 48 MHZ BUS CLOCK...
            void init(void);
            void delayMs(uint32_t ms);
        private:
};
} // namespace Drivers