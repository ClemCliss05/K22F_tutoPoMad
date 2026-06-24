#pragma once

class ILoggerBackend {
  public:
    virtual ~ILoggerBackend() = default;

    virtual void write(char c) = 0;
};