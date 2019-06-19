#pragma once

#include <cstdint>
#include <random>

namespace chip8 {
class random {
public:
  uint8_t gen();

private:
  static inline std::random_device                 rd{};
  static inline std::mt19937                       mt{rd()};
  static inline std::uniform_int_distribution<int> dist{0, 255};
};
}  // namespace chip8