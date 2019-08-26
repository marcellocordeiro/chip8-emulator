#include "cpu.h"

#include <algorithm>
#include <fstream>

#include "audio.h"
#include "emulator.h"
#include "input.h"
#include "io.h"

namespace chip8 {
cpu::cpu(chip8::emulator& emulator_ref) : emulator(emulator_ref)
{
  constexpr std::array<uint8_t, 80> font_set{
      0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
      0x20, 0x60, 0x20, 0x20, 0x70,  // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
      0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
      0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
      0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
      0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
      0xF0, 0x80, 0xF0, 0x80, 0x80   // F
  };

  std::copy(font_set.begin(), font_set.end(), memory.begin());
}

void cpu::power_on()
{
  reset();
}

void cpu::reset()
{
  emulator.get_io()->clear();

  std::fill(memory.begin() + 80, memory.end(), static_cast<uint8_t>(0));
  stack = {};
  V     = {};

  pc = 0x200;
  sp = 0;
  I  = 0;

  delay_timer = 0;
  sound_timer = 0;
}

void cpu::load(const std::filesystem::path& rom_file)
{
  auto rom_size = std::filesystem::file_size(rom_file);

  std::ifstream rom(rom_file, std::ios::binary);

  if (!rom) {
    throw std::runtime_error("Can't open the ROM");
  }

  rom.read(reinterpret_cast<char*>(memory.data()) + 0x200, rom_size);

  rom.close();
}

void cpu::cycle()
{
  if (cycle_timer.elapsed_time() >= 1.0f / ct::cpu_clock) {
    execute();
    cycle_timer.restart();
  }

  if (timers_timer.elapsed_time() >= 1.0f / ct::cpu_counters_clock) {
    emulator.get_input()->update_keys();
    emulator.get_io()->draw();

    if (delay_timer > 0) {
      --delay_timer;
    }

    if (sound_timer > 0) {
      if (sound_timer == 1) {
        emulator.get_audio()->play();
      }

      --sound_timer;
    }

    timers_timer.restart();
  }
}

void cpu::execute()
{
  const opcode_t opcode{
      static_cast<uint16_t>(memory[pc] << 8 | memory[pc + 1])};

  switch (opcode.main) {
    case 0x0: return opcode_0x0(opcode);
    case 0x1: return jmp_addr(opcode);
    case 0x2: return call_addr(opcode);
    case 0x3: return se_Vx_byte(opcode);
    case 0x4: return sne_Vx_byte(opcode);
    case 0x5: return se_Vx_Vy(opcode);
    case 0x6: return ld_Vx_byte(opcode);
    case 0x7: return add_Vx_byte(opcode);
    case 0x8: return opcode_0x8(opcode);
    case 0x9: return sne_Vx_Vy(opcode);
    case 0xA: return ld_I_addr(opcode);
    case 0xB: return jmp_V0_addr(opcode);
    case 0xC: return rnd_Vx_byte(opcode);
    case 0xD: return drw_Vx_Vy_nibble(opcode);
    case 0xE: return opcode_0xE(opcode);
    case 0xF: return opcode_0xF(opcode);
    default: throw std::runtime_error("opcode not found");
  }
}

void cpu::opcode_0x0(const opcode_t& opcode)
{
  switch (opcode.kk) {
    case 0xE0: return cls();
    case 0xEE: return ret();
    default: throw std::runtime_error("opcode not found");
  }
}

void cpu::opcode_0x8(const opcode_t& opcode)
{
  switch (opcode.n) {
    case 0x0: return ld_Vx_Vy(opcode);
    case 0x1: return or_Vx_Vy(opcode);
    case 0x2: return and_Vx_Vy(opcode);
    case 0x3: return xor_Vx_Vy(opcode);
    case 0x4: return add_Vx_Vy(opcode);
    case 0x5: return sub_Vx_Vy(opcode);
    case 0x6: return shr_Vx(opcode);
    case 0x7: return subn_Vx_Vy(opcode);
    case 0xE: return shl_Vx(opcode);
    default: throw std::runtime_error("opcode not found");
  }
}

void cpu::opcode_0xE(const opcode_t& opcode)
{
  switch (opcode.kk) {
    case 0x9E: return skp_Vx(opcode);
    case 0xA1: return sknp_Vx(opcode);
    default: throw std::runtime_error("opcode not found");
  }
}

void cpu::opcode_0xF(const opcode_t& opcode)
{
  switch (opcode.kk) {
    case 0x07: return ld_Vx_DT(opcode);
    case 0x0A: return ld_Vx_K(opcode);
    case 0x15: return ld_DT_Vx(opcode);
    case 0x18: return ld_ST_Vx(opcode);
    case 0x1E: return add_I_Vx(opcode);
    case 0x29: return ld_F_Vx(opcode);
    case 0x33: return ld_B_Vx(opcode);
    case 0x55: return ld_I_Vx(opcode);
    case 0x65: return ld_Vx_I(opcode);
    default: throw std::runtime_error("opcode not found");
  }
}

// CLS - clear display [0x00E0]
void cpu::cls()
{
  emulator.get_io()->clear();

  pc += 2;
}

// RET - return from a subroutine [0x00EE]
void cpu::ret()
{
  --sp;
  pc = stack[sp];

  pc += 2;
}

// JP addr - jump to location nnn [0x1nnn]
void cpu::jmp_addr(const opcode_t& opcode)
{
  pc = opcode.nnn;
}

// CALL addr - call subroutine at nnn [0x2nnn]
void cpu::call_addr(const opcode_t& opcode)
{
  stack[sp] = pc;
  ++sp;
  pc = opcode.nnn;
}

// SE Vx, byte - skip next instruction if Vx = kk [0x3xkk]
void cpu::se_Vx_byte(const opcode_t& opcode)
{
  if (V[opcode.x] == opcode.kk) {
    pc += 2;
  }

  pc += 2;
}

// SNE Vx, byte - skip next instruction if Vx != kk [0x4xkk]
void cpu::sne_Vx_byte(const opcode_t& opcode)
{
  if (V[opcode.x] != opcode.kk) {
    pc += 2;
  }

  pc += 2;
}

// SE Vx, Vy - skip next instruction if Vx = Vy [0x5xy0]
void cpu::se_Vx_Vy(const opcode_t& opcode)
{
  if (V[opcode.x] == V[opcode.y]) {
    pc += 2;
  }

  pc += 2;
}

// LD Vx, byte - set Vx = kk [0x6xkk]
void cpu::ld_Vx_byte(const opcode_t& opcode)
{
  V[opcode.x] = static_cast<uint8_t>(opcode.kk);

  pc += 2;
}

// ADD Vx, byte - set Vx = Vx + kk [0x7xkk]
void cpu::add_Vx_byte(const opcode_t& opcode)
{
  V[opcode.x] += static_cast<uint8_t>(opcode.kk);

  pc += 2;
}

// LD Vx, Vy - set Vx = Vy [0x8xy0]
void cpu::ld_Vx_Vy(const opcode_t& opcode)
{
  V[opcode.x] = V[opcode.y];

  pc += 2;
}

// OR Vx, Vy - set Vx = Vx OR Vy [0x8xy1]
void cpu::or_Vx_Vy(const opcode_t& opcode)
{
  V[opcode.x] |= V[opcode.y];

  pc += 2;
}

// AND Vx, Vy - set Vx = Vx AND Vy [0x8xy2]
void cpu::and_Vx_Vy(const opcode_t& opcode)
{
  V[opcode.x] &= V[opcode.y];

  pc += 2;
}

// XOR Vx, Vy - set Vx = Vx XOR Vy [0x8xy3]
void cpu::xor_Vx_Vy(const opcode_t& opcode)
{
  V[opcode.x] ^= V[opcode.y];

  pc += 2;
}

// ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry [0x8xy4]
void cpu::add_Vx_Vy(const opcode_t& opcode)
{
  V[0xF] = V[opcode.y] > (0xFF - V[opcode.x]);
  V[opcode.x] += V[opcode.y];

  pc += 2;
}

// SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow [0x8xy5]
void cpu::sub_Vx_Vy(const opcode_t& opcode)
{
  V[0xF] = V[opcode.x] >= V[opcode.y];
  V[opcode.x] -= V[opcode.y];

  pc += 2;
}

// SHR Vx {,Vy} - set Vx = Vx SHR 1 [0x8xy6]
void cpu::shr_Vx(const opcode_t& opcode)
{
  V[0xF] = V[opcode.x] & 0x1;
  V[opcode.x] >>= 1;

  pc += 2;
}

// SUBN Vx, Vy - set Vx = Vy - Vx, set VF = NOT borrow [0x8xy7]
void cpu::subn_Vx_Vy(const opcode_t& opcode)
{
  V[0xF]      = V[opcode.y] >= V[opcode.x];
  V[opcode.x] = V[opcode.y] - V[opcode.x];

  pc += 2;
}

// SHL Vx {,Vy} - set Vx = Vx SHL 1 [0x8xyE]
void cpu::shl_Vx(const opcode_t& opcode)
{
  V[0xF] = V[opcode.x] >> 7;
  V[opcode.x] <<= 1;

  pc += 2;
}

// SNE Vx, Vy - skip next instruction if Vx != Vy [0x9xy0]
void cpu::sne_Vx_Vy(const opcode_t& opcode)
{
  if (V[opcode.x] != V[opcode.y]) {
    pc += 2;
  }

  pc += 2;
}

// LD I, addr - set I = nnn [0xAnnn]
void cpu::ld_I_addr(const opcode_t& opcode)
{
  I = opcode.nnn;

  pc += 2;
}

// JP V0, addr - jump to location nnn + V0 [0xBnnn]
void cpu::jmp_V0_addr(const opcode_t& opcode)
{
  pc = opcode.nnn + V[0x0];
}

// RND Vx, byte - set Vx = random byte AND kk [0xCxkk]
void cpu::rnd_Vx_byte(const opcode_t& opcode)
{
  V[opcode.x] = opcode.kk & rng.gen();

  pc += 2;
}

// DRW Vx, Vy, nibble - display n-byte sprite starting at memory location I at
// (Vx, Vy), set VF = collision. [0xDxyn]
void cpu::drw_Vx_Vy_nibble(const opcode_t& opcode)
{
  uint8_t height = static_cast<uint8_t>(opcode.n);

  for (uint8_t i = 0; i < height; ++i) {
    uint8_t pixel = memory[I + i];
    uint8_t y     = (V[opcode.y] + i) % ct::screen_height;

    for (uint8_t j = 0; j < 8; ++j) {
      uint8_t x = (V[opcode.x] + j) % ct::screen_width;

      if ((pixel & (0x80 >> j)) != 0) {
        auto current_pixel = emulator.get_io()->get_pixel(x, y);
        V[0xF]             = (current_pixel == ct::main_colour);

        if (current_pixel == ct::main_colour) {
          emulator.get_io()->set_pixel(x, y, ct::background_colour);
        } else {
          emulator.get_io()->set_pixel(x, y, ct::main_colour);
        }
      }
    }
  }

  pc += 2;
}

// SKP Vx - skip next instruction if key
// with the value of Vx is pressed [0xEx9E]
void cpu::skp_Vx(const opcode_t& opcode)
{
  if ((*emulator.get_input())[V[opcode.x]]) {
    pc += 2;
  }

  pc += 2;
}

// SKNP Vx - skip next instruction if key
// with the value of Vx is not pressed
// [0xExA1]
void cpu::sknp_Vx(const opcode_t& opcode)
{
  if (!(*emulator.get_input())[V[opcode.x]]) {
    pc += 2;
  }

  pc += 2;
}

// LD Vx, DT - set Vx = delay timer value [0xFx07]
void cpu::ld_Vx_DT(const opcode_t& opcode)
{
  V[opcode.x] = delay_timer;

  pc += 2;
}

// LD Vx, K - wait for a key press, store the value of the key in Vx [0xFx0A]
void cpu::ld_Vx_K(const opcode_t& opcode)
{
  for (uint8_t i = 0; i < ct::keys_size; ++i) {
    if ((*emulator.get_input())[i]) {
      V[opcode.x] = i;
      pc += 2;
      return;
    }
  }
}

// LD DT, Vx - set delay timer = Vx [0xFx15]
void cpu::ld_DT_Vx(const opcode_t& opcode)
{
  delay_timer = V[opcode.x];

  pc += 2;
}

// LD ST, Vx - set sound timer = Vx [0xFx18]
void cpu::ld_ST_Vx(const opcode_t& opcode)
{
  sound_timer = V[opcode.x];

  pc += 2;
}

// ADD I, Vx - set I = I + Vx [0xFx1E]
void cpu::add_I_Vx(const opcode_t& opcode)
{
  // V[0xF] = (I + V[opcode.x] > 0xFFF);
  I += V[opcode.x];

  pc += 2;
}

// LD F, Vx - set I = location of sprite for digit Vx [0xFx29]
void cpu::ld_F_Vx(const opcode_t& opcode)
{
  I = V[opcode.x] * ct::char_size;

  pc += 2;
}

// LD B, Vx - store BCD representation of Vx
// in memory locations I, I + 1, and I + 2 [0xFx33]
void cpu::ld_B_Vx(const opcode_t& opcode)
{
  memory[I]     = V[opcode.x] / 100;
  memory[I + 1] = (V[opcode.x] / 10) % 10;
  memory[I + 2] = V[opcode.x] % 10;

  pc += 2;
}

// LD [I], Vx - store registers V0 through Vx
// in memory starting at location I [0xFx55]
void cpu::ld_I_Vx(const opcode_t& opcode)
{
  for (int i = 0; i <= opcode.x; ++i) {
    memory[I + i] = V[i];
  }

  pc += 2;
}

// LD Vx, [I] - read registers V0 through Vx
// from memory starting at location I [0xFx65]
void cpu::ld_Vx_I(const opcode_t& opcode)
{
  for (int i = 0; i <= opcode.x; ++i) {
    V[i] = memory[I + i];
  }

  pc += 2;
}
}  // namespace chip8
