#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "common.hpp"
#include "input.h"
#include "random.h"

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(disable : 4201)  // nonstandard extension used: nameless struct/union
#pragma warning(disable : 4834)  // discarding return value of function with 'nodiscard' attribute
#endif

namespace chip8 {
class cpu {
public:
  cpu();
  void load(const std::string&);
  void cycle();
  void cycle_timers();
  const std::array<bool, cte::gfx_size>& get_gfx() const;
  void update_keys();
  bool get_draw() const;
  bool get_beep() const;

private:
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

  // gfx
  std::array<bool, cte::gfx_size> _gfx = {};

  // memory
  std::array<uint8_t, cte::memory_size> _memory = {};
  uint16_t _pc = 0x200;  // program counter

  // stack
  std::array<uint16_t, cte::stack_size> _stack = {};
  uint8_t _sp = 0;  // stack pointer

  // keys
  chip8::input _keys = {};

  // misc registers
  std::array<uint8_t, cte::V_size> _V = {};
  uint16_t _I = 0;

  // timers
  uint8_t _delay_timer = 0;
  uint8_t _sound_timer = 0;

  // flags
  bool _draw = false;
  bool _beep = false;

  // random
  random _rng;

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