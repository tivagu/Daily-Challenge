#include "audio_ring_buffer.hpp"
#include <cstdio>
#include <cstdlib>

// constructor khai báo dung lượng + head và tail
// push với cộng dần vào index và nếu indexx vượt -> ta tính tổng đơn giản : index + 1 và chia lấy dư với capacity
// pop không nhất thiết phải có tham số nhưng ta dùng địa chỉ thì vẫn hợp lệ
AudioRingBuffer::AudioRingBuffer(std::size_t capacity) { // constructor
    capacity_ = capacity;
    head_ = 0;
    tail_ = 0;
    size_ = 0;
}

bool AudioRingBuffer::push(std::int16_t sample) { 
    if (size_ == capacity_){
        return false;
    }
    buffer_[tail_] = sample;
    tail_ = (tail_ + 1) % capacity_;
    size_++;
    return true;
}

bool AudioRingBuffer::pop(std::int16_t& sample) {
    if (size_ == 0){return false;}
    sample = buffer_[head_];
    head_ = (head_ + 1) % capacity_;
    size_--;
    return true;  
}

std::size_t AudioRingBuffer::size() const {
    return size_;
}
