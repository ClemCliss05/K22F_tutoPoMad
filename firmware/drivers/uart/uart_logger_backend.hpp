#pragma once

#include "logger_backend.hpp"
#include "uart.hpp"

class UartLoggerBackend : public ILoggerBackend {
    public:
      explicit UartLoggerBackend(Drivers::Uart &uart);

    void write(char c) override;

    private:
      Drivers::Uart &uart_;
};