#pragma once

namespace cte {
// cpu
constexpr auto cpu_clock = 840.0f;
// constexpr auto cpu_counters_clock = 60.f;

// graphics
constexpr auto scale = 20.0f;
constexpr auto res_scale = 20u;
constexpr auto fps_limit = 60u;
constexpr auto screen_width = 64;
constexpr auto screen_height = 32;
constexpr auto char_size = 5;
constexpr auto pixels_per_byte = 8;

// audio
constexpr auto frequency = 880.0f;
constexpr auto updateRate = 22100.0f;
constexpr auto amplitude = 16000.0f;
constexpr auto stepCount = updateRate / frequency;
constexpr auto step = (6.284f) / stepCount;

// array sizes
constexpr auto gfx_size = screen_width * screen_height;
constexpr auto keys_size = 16;
constexpr auto memory_size = 4096;
constexpr auto stack_size = 16;
constexpr auto V_size = 16;
}  // namespace cte