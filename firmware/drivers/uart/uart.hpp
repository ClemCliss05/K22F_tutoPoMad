#pragma once

#include <cstddef>
#include <cstdint>

namespace Drivers {
class Uart {
    public:
        void init();

        bool isTxReady() const;
        bool isRxReady() const;

        void writeByte(uint8_t byte);
        void write(const char *str);

        uint8_t readByte();

    private:
};
} // namespace Drivers