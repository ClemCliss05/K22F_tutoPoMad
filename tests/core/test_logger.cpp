#include <gtest/gtest.h>

#include "logger.hpp"
#include "logger_backend.hpp"
#include "ringbuffer.hpp"

#include <string>

class FakeBackend : public LoggerBackend {
  public:
    std::string out;

    void write(char c) override {
        out.push_back(c);
    }
};

TEST(LoggerTest, LogMessage) {
    char mem[64];

    RingBuffer rb(mem, 64);

    FakeBackend backend;

    Logger logger(rb, backend);

    logger.log("hello");

    logger.flush();

    EXPECT_EQ(backend.out, "hello");
}