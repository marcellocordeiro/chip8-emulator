#include "random.h"

namespace lib {
uint8_t random::gen()
{
  return static_cast<uint8_t>(dist(mt));
}
}  // namespace lib
