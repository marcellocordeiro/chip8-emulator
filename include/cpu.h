#pragma once

#include <array>
#include <filesystem>

#include "bit_field.hpp"
#include "common.h"

#include "audio.h"
#include "display.h"
#include "input.h"
#include "random.h"
#include "timer.h"

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
  cpu();

  void set_component(chip8::display&);
  void set_component(chip8::audio&);
  void set_component(chip8::input&);

  void reset();
  void load(const std::filesystem::path&);
  void cycle();

private:
  union opcode_t {
    template <size_t position, size_t bits>
    using bf_16 = lib::bit_field<uint16_t, position, bits>;

    uint16_t raw = 0;

    bf_16<12, 4> main;
    bf_16<8, 4>  x;
    bf_16<4, 4>  y;
    bf_16<0, 4>  n;
    bf_16<0, 12> nnn;
    bf_16<0, 8>  kk;
  };

  chip8::display* display = nullptr;
  chip8::audio*   sound   = nullptr;
  chip8::input*   keys    = nullptr;
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