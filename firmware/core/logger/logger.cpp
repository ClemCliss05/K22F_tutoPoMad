#include <cassert>
#include <cstdarg>
#include <cstdio>

#include "logger.hpp"

Logger *global_logger = nullptr;

Logger::Logger(RingBuffer &buffer, ILoggerBackend &backend) : buffer_(buffer), backend_(backend) {
    global_logger = this;
}

void Logger::log(LogLevel level, const char* fmt, ...) {
    // Format the message using vsnprintf
    char fmtMsg[128];
    const char* pFmtMsg = fmtMsg;
    va_list args;
    va_start(args, fmt);
    vsnprintf(fmtMsg,
          sizeof(fmtMsg),
          fmt,
          args);
    va_end(args);

    writePrefix(level);

    // Push the message into the ring buffer
    while (*pFmtMsg) {
        // Push each character into the ring buffer
        if (!buffer_.push(*pFmtMsg)) {
            // If the buffer is full, flush the buffer to the backend
            return;
        }
        ++pFmtMsg;
    }
    buffer_.push('\r');
    buffer_.push('\n');
    flush();
}

void Logger::writePrefix(LogLevel level)
{
    const char* prefix = "";

    switch(level)
    {
        case LogLevel::Error:
            prefix = "[ERROR] ";
            break;

        case LogLevel::Warn:
            prefix = "[WARN ] ";
            break;

        case LogLevel::Info:
            prefix = "[INFO ] ";
            break;

        case LogLevel::Debug:
            prefix = "[DEBUG] ";
            break;
    }

    while (*prefix)
    {
        if (!buffer_.push(*prefix)) {
            return;
        }
        ++prefix;
    }
}

void Logger::flush() {
    char c;

    while (buffer_.pop(c)) {
        backend_.write(c);
    }
}