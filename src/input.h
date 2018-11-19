#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <unordered_map>

#include "common.hpp"

namespace chip8 {
class input {
public:
  input();
  void update_keys();
  bool operator[](std::size_t);

private:
  std::array<sf::Keyboard::Key, cte::keys_size> mapping;
  std::array<bool, cte::keys_size> keys;
};
}  // namespace chip8