#include "logger.hpp"
#include <cstring>

Logger *global_logger = nullptr;

Logger::Logger(RingBuffer &buffer, LoggerBackend &backend) : buffer_(buffer), backend_(backend) {
    global_logger = this;
}

void Logger::log(const char *msg) {
    buffer_.write(msg, strlen(msg));
}

void Logger::flush() {
    char c;

    while (buffer_.pop(c)) {
        backend_.write(c);
    }
}