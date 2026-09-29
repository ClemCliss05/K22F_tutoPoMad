#pragma once

#include <cstddef>
#include <cstdint>

namespace Drivers {
class Uart {
  public:
    /*
     * LED driver functions
     * PTE0 -> UART1_TX
     * PTE1 -> UART1_RX
     */

    void init();

    bool isTxReady() const;
    bool isRxReady() const;

    void writeByte(uint8_t byte);
    void write(const char *str);

    uint8_t readByte();

  private:
};
} // namespace Drivers