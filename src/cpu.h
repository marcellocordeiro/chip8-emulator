#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "display.h"
#include "input.h"
#include "random.h"
#include "sound.h"
#include "timer.h"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(disable : 4201)  // nonstandard extension used: nameless struct/union
#pragma warning(disable : 4834)  // discarding return value of function with 'nodiscard' attribute
#endif

namespace ct {
// cpu
constexpr auto cpu_clock = 840.0f;
constexpr auto cpu_counters_clock = 60.0f;

// graphics
constexpr auto screen_width = 64;
constexpr auto screen_height = 32;
constexpr auto char_size = 5;
constexpr auto pixels_per_byte = 8;

// array sizes
constexpr auto memory_size = 4096;
constexpr auto stack_size = 16;
constexpr auto V_size = 16;

// palette
const auto background_colour = sf::Color::Black;
const auto main_colour = sf::Color::White;
}  // namespace ct

namespace chip8 {
class cpu {
public:
  cpu(chip8::display&, chip8::sound&, chip8::input&);
  void reset();
  void load(const std::filesystem::path&);
  void cycle();
  void cycle_timers();

private:
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif defined(__clang__) && !defined(_MSC_VER)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#endif
  // opcode splitting
  union opcode_t {
    uint16_t raw;

    struct {
      uint16_t nnn : 12;
      uint16_t main : 4;
    };

    uint8_t kk : 8;

    struct {
      uint8_t n : 4;
      uint8_t y : 4;
      uint8_t x : 4;
    };
  };
#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif defined(__clang__) && !defined(_MSC_VER)
#pragma clang diagnostic pop
#endif

  // display
  chip8::display& _display;

  // sound
  chip8::sound& _sound;

  // keys
  chip8::input& _keys;

  // stopwatches
  chip8::timer _cycle_timer;
  chip8::timer _timers_timer;

  // memory
  std::array<uint8_t, ct::memory_size> _memory = {};
  uint16_t _pc = 0x200;  // program counter

  // stack
  std::array<uint16_t, ct::stack_size> _stack = {};
  uint8_t _sp = 0;  // stack pointer

  // misc registers
  std::array<uint8_t, ct::V_size> _V = {};
  uint16_t _I = 0;

  // timers
  uint8_t _delay_timer = 0;
  uint8_t _sound_timer = 0;

  // random
  chip8::random _rng;

  // instructions
  void cls();
  void ret();
  void jmp_addr(opcode_t opcode);
  void call_addr(opcode_t opcode);
  void se_Vx_byte(opcode_t opcode);
  void sne_Vx_byte(opcode_t opcode);
  void se_Vx_Vy(opcode_t opcode);
  void ld_Vx_byte(opcode_t opcode);
  void add_Vx_byte(opcode_t opcode);
  void ld_Vx_Vy(opcode_t opcode);
  void or_Vx_Vy(opcode_t opcode);
  void and_Vx_Vy(opcode_t opcode);
  void xor_Vx_Vy(opcode_t opcode);
  void add_Vx_Vy(opcode_t opcode);
  void sub_Vx_Vy(opcode_t opcode);
  void shr_Vx(opcode_t opcode);
  void subn_Vx_Vy(opcode_t opcode);
  void shl_Vx(opcode_t opcode);
  void sne_Vx_Vy(opcode_t opcode);
  void ld_I_addr(opcode_t opcode);
  void jmp_V0_addr(opcode_t opcode);
  void rnd_Vx_byte(opcode_t opcode);
  void drw_Vx_Vy_nibble(opcode_t opcode);
  void skp_Vx(opcode_t opcode);
  void sknp_Vx(opcode_t opcode);
  void ld_Vx_DT(opcode_t opcode);
  void ld_Vx_K(opcode_t opcode);
  void ld_DT_Vx(opcode_t opcode);
  void ld_ST_Vx(opcode_t opcode);
  void add_I_Vx(opcode_t opcode);
  void ld_F_Vx(opcode_t opcode);
  void ld_B_Vx(opcode_t opcode);
  void ld_I_Vx(opcode_t opcode);
  void ld_Vx_I(opcode_t opcode);
  void opcode_0x0(opcode_t opcode);
  void opcode_0x8(opcode_t opcode);
  void opcode_0xE(opcode_t opcode);
  void opcode_0xF(opcode_t opcode);
};
}  // namespace chip8