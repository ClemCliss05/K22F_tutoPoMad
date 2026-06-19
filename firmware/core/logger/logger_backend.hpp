#pragma once

#include <cstddef>

class LoggerBackend {
  public:
    virtual ~LoggerBackend() = default;

    virtual void write(char c) = 0;
};