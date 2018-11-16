#include "random.h"

/* auto init() {
  std::array<int, std::mt19937::state_size> seed_data;
  std::random_device r;
  std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
  std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

  return std::mt19937(seq);
} */

uint8_t random::gen() {
  return static_cast<uint8_t>(dist(mt));
}