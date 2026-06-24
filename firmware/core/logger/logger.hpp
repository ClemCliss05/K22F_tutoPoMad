#pragma once

#include "logger_backend.hpp"
#include "ringbuffer.hpp"

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

class Logger {
  public:
    Logger(RingBuffer &buffer, ILoggerBackend &backend);

    void log(const char *msg);

    void flush();

  private:
    RingBuffer &buffer_;
    ILoggerBackend &backend_;
};

extern Logger *global_logger;

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(msg)                                                                             \
    do {                                                                                           \
        if (global_logger)                                                                         \
            global_logger->log(msg);                                                               \
    } while (0)
#else
#define LOG_ERROR(msg)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN(msg)                                                                              \
    do {                                                                                           \
        if (global_logger)                                                                         \
            global_logger->log(msg);                                                               \
    } while (0)
#else
#define LOG_WARN(msg)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(msg)                                                                              \
    do {                                                                                           \
        if (global_logger)                                                                         \
            global_logger->log(msg);                                                               \
    } while (0)
#else
#define LOG_INFO(msg)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(msg)                                                                             \
    do {                                                                                           \
        if (global_logger)                                                                         \
            global_logger->log(msg);                                                               \
    } while (0)
#else
#define LOG_DEBUG(msg)
#endif