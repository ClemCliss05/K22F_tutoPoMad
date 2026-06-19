#pragma once

#include <cstddef>

class RingBuffer {
  public:
    RingBuffer(char *buffer, size_t size);

    bool push(char c);
    bool pop(char &c);

    bool write(const char *data, size_t len);

  private:
    char *buffer_;
    size_t size_;

    size_t head_;
    size_t tail_;
};