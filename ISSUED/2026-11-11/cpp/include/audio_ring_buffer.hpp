#pragma once
#include <cstddef>
#include <cstdint>

class AudioRingBuffer {
public:
    explicit AudioRingBuffer(std::size_t capacity);
    bool push(std::int16_t sample);
    bool pop(std::int16_t& sample);
    std::size_t size() const;

private:
    std::int16_t buffer_[32] {};
    std::size_t capacity_ = 0; // user's storage 
    std::size_t head_ = 0; 
    std::size_t tail_ = 0;
    std::size_t size_ = 0;
};
