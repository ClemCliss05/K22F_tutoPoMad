#pragma once

#include <cstddef>
#include <cstdint>

namespace Drivers {
/**
 * Configures and controls a K22F UART peripheral.
 */
class Uart {
  public:
    /*
     * Pin PTE0 -> UART1_TX
     * Pin PTE1 -> UART1_RX
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