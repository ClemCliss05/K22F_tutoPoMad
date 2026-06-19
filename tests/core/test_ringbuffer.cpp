#include "ringbuffer.hpp"
#include <gtest/gtest.h>

TEST(RingBufferTest, PushPop) {
    char mem[10];

    RingBuffer rb(mem, 10);

    EXPECT_TRUE(rb.push('a'));

    char c;
    EXPECT_TRUE(rb.pop(c));

    EXPECT_EQ(c, 'a');
}