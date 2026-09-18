#pragma once

#include "logger_backend.hpp"
#include "uart.hpp"

namespace Services {

class UartLoggerBackend : public ILoggerBackend {
  public:
    /**
     * Use an existing UART driver as the logger output.
     */
    explicit UartLoggerBackend(Drivers::Uart &uart);

    void write(char c) override;

  private:
    Drivers::Uart &uart_;
};

} // namespace Services