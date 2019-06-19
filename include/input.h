#pragma once

#include <array>
#include <unordered_map>

#include <SFML/Window/Keyboard.hpp>

namespace ct {
constexpr auto keys_size = 16;
}  // namespace ct

namespace chip8 {
class input {
public:
  input();
  void update_keys();
  bool operator[](const std::size_t) const;

private:
  std::array<sf::Keyboard::Key, ct::keys_size> mapping;
  std::array<bool, ct::keys_size>              keys;
};
}  // namespace chip8