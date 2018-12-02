#pragma once

#include <chrono>

namespace chip8 {
class timer {
public:
  float elapsed_time();
  void restart();

private:
  std::chrono::high_resolution_clock::time_point _start = std::chrono::high_resolution_clock::now();
};
}  // namespace chip8