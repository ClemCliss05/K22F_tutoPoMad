#pragma once

#include "logger_backend.hpp"

class UartLoggerBackend : public LoggerBackend {
  public:
    void write(char c) override;
};