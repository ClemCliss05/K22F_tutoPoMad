#include <gtest/gtest.h>

#include "logger.hpp"
#include "logger_backend.hpp"
#include "ringbuffer.hpp"

#include <string>

class FakeBackend : public ILoggerBackend {
  public:
    std::string out;

    void write(char c) override {
        out.push_back(c);
    }
};

TEST(LoggerTest, LogMessage) {
    char mem[64];

    RingBuffer rb(mem, sizeof(mem));

    FakeBackend backend;

    Logger logger(rb, backend);

    logger.log(LogLevel::Debug,"hello");

    EXPECT_EQ( backend.out, "[DEBUG] hello\r\n");
}

TEST(LoggerTest, FormatInteger) {
    char mem[64];

    RingBuffer rb(mem, sizeof(mem));

    FakeBackend backend;

    Logger logger(rb, backend);

    logger.log(LogLevel::Debug, "Counter=%d", 42);

    EXPECT_EQ( backend.out, "[DEBUG] Counter=42\r\n");
}

TEST(LoggerTest, MultipleMessages) {
    char mem[64];

    RingBuffer rb(mem, sizeof(mem));

    FakeBackend backend;

    Logger logger(rb, backend);

    logger.log(LogLevel::Debug, "Hello");
    logger.log(LogLevel::Debug, "World");

    EXPECT_EQ( backend.out, "[DEBUG] Hello\r\n" "[DEBUG] World\r\n");
}