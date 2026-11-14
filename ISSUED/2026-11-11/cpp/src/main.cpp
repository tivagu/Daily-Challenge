// Supplied single-threaded host replay driver.
#include "audio_ring_buffer.hpp"
#include <iostream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::size_t capacity = 0, count = 0;
    if (!(std::cin >> capacity >> count)) {
        return 1;
    }
    AudioRingBuffer ring(capacity);
    for (std::size_t i = 0; i < count; ++i) {
        std::string command;
        if (!(std::cin >> command)) {
            return 1;
        }
        if (command == "PUSH") {
            int sample = 0;
            if (!(std::cin >> sample)) {
                return 1;
            }
            std::cout << (ring.push(static_cast<std::int16_t>(sample)) ? "OK" : "FULL") << '\n';
        } else if (command == "POP") {
            std::int16_t sample = 0;
            if (ring.pop(sample)) {
                std::cout << static_cast<int>(sample) << '\n';
            } else {
                std::cout << "EMPTY\n";
            }
        } else if (command == "SIZE") {
            std::cout << ring.size() << '\n';
        } else {
            return 1;
        }
    }
}
