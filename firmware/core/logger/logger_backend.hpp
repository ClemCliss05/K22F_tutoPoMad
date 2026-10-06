#pragma once

/**
 * Interface for logger output backends.
 */
class ILoggerBackend {
  public:
    virtual ~ILoggerBackend() = default;

    virtual void write(char c) = 0;
};