#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>

#include "audio.h"
#include "display.h"
#include "input.h"
#include "random.h"
#include "timer.h"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(disable : 4201)  // nonstandard extension used: nameless struct/union
#pragma warning(disable : 4834)  // discarding return value of function with 'nodiscard' attribute
#endif

namespace ct {
// cpu
constexpr auto cpu_clock          = 840.0f;
constexpr auto cpu_counters_clock = 60.0f;

// graphics
constexpr auto screen_width    = 64;
constexpr auto screen_height   = 32;
constexpr auto char_size       = 5;
constexpr auto pixels_per_byte = 8;

// array sizes
constexpr auto memory_size = 4096;
constexpr auto stack_size  = 16;
constexpr auto V_size      = 16;

// palette
constexpr auto background_colour = 0x000000FF;
constexpr auto main_colour       = 0xFFFFFFFF;
}  // namespace ct

namespace chip8 {
class cpu {
public:
  cpu(chip8::display&, chip8::audio&, chip8::input&);
  void reset();
  void load(const std::filesystem::path&);
  void cycle();

private:
  struct opcode_t {
    uint8_t  main : 4;
    uint8_t  x : 4;
    uint8_t  y : 4;
    uint8_t  n : 4;
    uint16_t nnn : 12;
    uint8_t  kk : 8;

    opcode_t(const uint16_t raw) {
      main = (raw & 0xF000) >> 12;
      x    = (raw & 0x0F00) >> 8;
      y    = (raw & 0x00F0) >> 4;
      n    = raw & 0x000F;
      nnn  = raw & 0x0FFF;
      kk   = raw & 0x00FF;
    }
  };

  chip8::display& display;
  chip8::audio&   sound;
  chip8::input&   keys;
  chip8::timer    cycle_timer;
  chip8::timer    timers_timer;
  chip8::random   rng;

  std::array<uint8_t, ct::memory_size> memory = {};
  std::array<uint16_t, ct::stack_size> stack  = {};
  std::array<uint8_t, ct::V_size>      V      = {};

  uint16_t pc          = 0x200;
  uint8_t  sp          = 0;
  uint16_t I           = 0;
  uint8_t  delay_timer = 0;
  uint8_t  sound_timer = 0;

  void execute();

  void cls();
  void ret();
  void jmp_addr(const opcode_t&);
  void call_addr(const opcode_t&);
  void se_Vx_byte(const opcode_t&);
  void sne_Vx_byte(const opcode_t&);
  void se_Vx_Vy(const opcode_t&);
  void ld_Vx_byte(const opcode_t&);
  void add_Vx_byte(const opcode_t&);
  void ld_Vx_Vy(const opcode_t&);
  void or_Vx_Vy(const opcode_t&);
  void and_Vx_Vy(const opcode_t&);
  void xor_Vx_Vy(const opcode_t&);
  void add_Vx_Vy(const opcode_t&);
  void sub_Vx_Vy(const opcode_t&);
  void shr_Vx(const opcode_t&);
  void subn_Vx_Vy(const opcode_t&);
  void shl_Vx(const opcode_t&);
  void sne_Vx_Vy(const opcode_t&);
  void ld_I_addr(const opcode_t&);
  void jmp_V0_addr(const opcode_t&);
  void rnd_Vx_byte(const opcode_t&);
  void drw_Vx_Vy_nibble(const opcode_t&);
  void skp_Vx(const opcode_t&);
  void sknp_Vx(const opcode_t&);
  void ld_Vx_DT(const opcode_t&);
  void ld_Vx_K(const opcode_t&);
  void ld_DT_Vx(const opcode_t&);
  void ld_ST_Vx(const opcode_t&);
  void add_I_Vx(const opcode_t&);
  void ld_F_Vx(const opcode_t&);
  void ld_B_Vx(const opcode_t&);
  void ld_I_Vx(const opcode_t&);
  void ld_Vx_I(const opcode_t&);
  void opcode_0x0(const opcode_t&);
  void opcode_0x8(const opcode_t&);
  void opcode_0xE(const opcode_t&);
  void opcode_0xF(const opcode_t&);
};
}  // namespace chip8