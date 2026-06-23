#pragma once

#include "logger_backend.hpp"

namespace Drivers::Uart
{
    class UartLoggerBackend : public LoggerBackend {
      public:
        void write(char c) override;
    };
}