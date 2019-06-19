#include "random.h"

namespace chip8 {
uint8_t random::gen()
{
  return static_cast<uint8_t>(dist(mt));
}
}  // namespace chip8