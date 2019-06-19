#pragma once

#include <array>
#include <queue>

#include <SFML/Graphics.hpp>

namespace ct {
// array sizes
constexpr auto gfx_width  = 64;
constexpr auto gfx_height = 32;

// graphics
constexpr auto scale     = 20.0f;
constexpr auto res_scale = 20u;
constexpr auto fps_limit = 60u;

constexpr auto res_width  = gfx_width * res_scale;
constexpr auto res_height = gfx_height * res_scale;
}  // namespace ct

enum class action_type { pause, reset };

namespace chip8 {
class display {
public:
  display();
  bool is_open();
  void close();
  void clear();

  std::queue<action_type>& poll_events();

  std::uint32_t get_pixel(const std::size_t, const std::size_t) const;
  void set_pixel(const std::size_t, const std::size_t, const std::uint32_t);

  void render();

private:
  sf::RenderWindow        window;
  std::queue<action_type> actions;

  std::array<std::array<sf::Color, ct::gfx_height>, ct::gfx_width> gfx = {{}};
};
}  // namespace chip8