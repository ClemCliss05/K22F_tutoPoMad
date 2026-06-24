#include "logger.hpp"

Logger *global_logger = nullptr;

Logger::Logger(RingBuffer &buffer, ILoggerBackend &backend) : buffer_(buffer), backend_(backend) {
    global_logger = this;
}

void Logger::log(const char *msg) {
    while (*msg) {
        buffer_.push(*msg);
        ++msg;
    }
}

void Logger::flush() {
    char c;

    while (buffer_.pop(c)) {
        backend_.write(c);
    }
}