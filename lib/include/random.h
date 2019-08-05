#pragma once

#include <random>

#include "common.h"

namespace lib {
class random {
public:
  uint8_t gen();

private:
  static inline std::random_device                 rd{};
  static inline std::mt19937                       mt{rd()};
  static inline std::uniform_int_distribution<int> dist{0, 255};
};
}  // namespace lib
