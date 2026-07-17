#include "ringbuffer.hpp"
#include <gtest/gtest.h>

TEST(RingBufferTest, PushPop) {
    char mem[10];

    RingBuffer rb(mem, sizeof(mem));

    EXPECT_TRUE(rb.push('a'));

    char c;
    EXPECT_TRUE(rb.pop(c));

    EXPECT_EQ(c, 'a');
}

TEST(RingBufferTest, EmptyBuffer) {
    char mem[10];

    RingBuffer rb(mem, sizeof(mem));

    char c;
    EXPECT_FALSE(rb.pop(c));
}

TEST(RingBufferTest, FullBuffer) {
    char mem[10];

    RingBuffer rb(mem, sizeof(mem));
    for (int i = 0; i < 9; ++i) {
        EXPECT_TRUE(rb.push('a' + i));
    }

    EXPECT_FALSE(rb.push('x'));
}