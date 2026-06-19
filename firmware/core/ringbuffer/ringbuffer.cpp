#include "ringbuffer.hpp"

RingBuffer::RingBuffer(char *buffer, size_t size)
    : buffer_(buffer), size_(size), head_(0), tail_(0) {}

bool RingBuffer::push(char c) {
    // Calculate the next head position
    size_t next = (head_ + 1) % size_;

    // buffer full
    if (next == tail_)
        return false;

    buffer_[head_] = c;
    head_ = next;

    return true;
}

bool RingBuffer::pop(char &c) {
    // buffer empty
    if (head_ == tail_)
        return false;

    c = buffer_[tail_];
    tail_ = (tail_ + 1) % size_;

    return true;
}

bool RingBuffer::write(const char *data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (!push(data[i]))
            return false;
    }

    return true;
}