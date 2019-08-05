#pragma once

#include <array>

#include <SFML/Window/Keyboard.hpp>

#include "common.h"

namespace ct {
constexpr auto keys_size = 16;
}  // namespace ct

namespace chip8 {
class input {
public:
  input();
  void update_keys();
  bool operator[](size_t) const;

private:
  std::array<sf::Keyboard::Key, ct::keys_size> mapping;
  std::array<bool, ct::keys_size>              keys;
};
}  // namespace chip8
