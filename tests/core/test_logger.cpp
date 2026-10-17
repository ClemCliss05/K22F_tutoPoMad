#include <gtest/gtest.h>

#include "logger.hpp"
#include "logger_backend.hpp"
#include "ringbuffer.hpp"

#include <string>

class FakeBackend : public Core::ILoggerBackend {
  public:
    std::string out;

    void write(char c) override {
        out.push_back(c);
    }
};

TEST(LoggerTest, LogMessage) {
    char mem[64];

    Core::RingBuffer rb(mem, sizeof(mem));

    FakeBackend backend;

    Core::Logger logger(rb, backend);

    logger.log(Core::LogLevel::Debug, "hello");

    EXPECT_EQ(backend.out, "[DEBUG] hello\r\n");
}

TEST(LoggerTest, FormatInteger) {
    char mem[64];

    Core::RingBuffer rb(mem, sizeof(mem));

    FakeBackend backend;

    Core::Logger logger(rb, backend);

    logger.log(Core::LogLevel::Debug, "Counter=%d", 42);

    EXPECT_EQ(backend.out, "[DEBUG] Counter=42\r\n");
}

TEST(LoggerTest, MultipleMessages) {
    char mem[64];

    Core::RingBuffer rb(mem, sizeof(mem));

    FakeBackend backend;

    Core::Logger logger(rb, backend);

    logger.log(Core::LogLevel::Debug, "Hello");
    logger.log(Core::LogLevel::Debug, "World");

    EXPECT_EQ(backend.out, "[DEBUG] Hello\r\n"
                           "[DEBUG] World\r\n");
}