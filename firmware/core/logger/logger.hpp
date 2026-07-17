#pragma once

#include "logger_backend.hpp"
#include "ringbuffer.hpp"

enum class LogLevel{
    Error,
    Warn,
    Info,
    Debug
};

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_INFO  2
#define LOG_LEVEL_DEBUG 3

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

class Logger {
  public:
    Logger(RingBuffer &buffer, ILoggerBackend &backend);

    void log(LogLevel level,
             const char* fmt,
             ...);

  private:
    void writePrefix(LogLevel level);
    void flush();

    RingBuffer &buffer_;
    ILoggerBackend &backend_;
};

extern Logger *global_logger;

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define LOG_ERROR(...)                                \
    do                                                \
    {                                                 \
        if(global_logger)                             \
            global_logger->log(                       \
                LogLevel::Error,                      \
                __VA_ARGS__);                         \
    } while(0)
#else
#define LOG_ERROR(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
#define LOG_WARN(...)                                 \
    do                                                \
    {                                                 \
        if(global_logger)                             \
            global_logger->log(                       \
                LogLevel::Warn,                       \
                __VA_ARGS__);                         \
    } while(0)
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define LOG_INFO(...)                                 \
    do                                                \
    {                                                 \
        if(global_logger)                             \
            global_logger->log(                       \
                LogLevel::Info,                       \
                __VA_ARGS__);                         \
    } while(0)
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#define LOG_DEBUG(...)                                \
    do                                                \
    {                                                 \
        if(global_logger)                             \
            global_logger->log(                       \
                LogLevel::Debug,                      \
                __VA_ARGS__);                         \
    } while(0)
#else
#define LOG_DEBUG(...)
#endif