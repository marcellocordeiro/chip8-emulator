#include "cpu.h"

namespace chip8 {
void print_error(const std::string& error, uint16_t opcode = 0x0000) {
  std::stringstream ss;
  ss << std::hex << std::showbase << std::internal << std::setfill('0') << +opcode;
  throw std::runtime_error(error + ss.str());
}

cpu::cpu(chip8::display& display, chip8::sound& sound, chip8::input& keys) : _display(display), _sound(sound), _keys(keys) {
  const std::array<uint8_t, 80> font_set = {
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

  std::copy(font_set.begin(), font_set.end(), _memory.begin());
}

void cpu::reset() {
  _display.clear();

  std::fill(_memory.begin() + 80, _memory.end(), 0);
  _stack.fill(0);
  _V.fill(0);

  _pc = 0x200;
  _sp = 0;
  _I = 0;

  _delay_timer = 0;
  _sound_timer = 0;
}

void cpu::load(const std::filesystem::path& rom_file) {
  auto rom_size = std::filesystem::file_size(rom_file);

  std::ifstream rom(rom_file, std::ios::binary);

  rom.read(reinterpret_cast<char*>(_memory.data() + 0x200), rom_size);

  rom.close();
}

void cpu::cycle() {
  if (!(_cycle_timer.elapsed_time() >= 1.0f / ct::cpu_clock)) {
    return;
  }

  const uint16_t raw_opcode = _memory[_pc] << 8 | _memory[_pc + 1];
  const opcode_t opcode = {raw_opcode};

  // std::cout << "[opcode] " << std::hex << std::showbase << std::internal << std::setfill('0') << +opcode.raw << std::endl;

  switch (opcode.main) {
    case 0x0:
      opcode_0x0(opcode);
      break;
    case 0x1:
      jmp_addr(opcode);
      break;
    case 0x2:
      call_addr(opcode);
      break;
    case 0x3:
      se_Vx_byte(opcode);
      break;
    case 0x4:
      sne_Vx_byte(opcode);
      break;
    case 0x5:
      se_Vx_Vy(opcode);
      break;
    case 0x6:
      ld_Vx_byte(opcode);
      break;
    case 0x7:
      add_Vx_byte(opcode);
      break;
    case 0x8:
      opcode_0x8(opcode);
      break;
    case 0x9:
      sne_Vx_Vy(opcode);
      break;
    case 0xA:
      ld_I_addr(opcode);
      break;
    case 0xB:
      jmp_V0_addr(opcode);
      break;
    case 0xC:
      rnd_Vx_byte(opcode);
      break;
    case 0xD:
      drw_Vx_Vy_nibble(opcode);
      break;
    case 0xE:
      opcode_0xE(opcode);
      break;
    case 0xF:
      opcode_0xF(opcode);
      break;
    default:
      print_error("[opcode not found]", opcode.raw);
      break;
  }

  _cycle_timer.restart();
}

void cpu::cycle_timers() {
  if (!(_timers_timer.elapsed_time() >= 1.0f / ct::cpu_counters_clock)) {
    return;
  }

  _keys.update_keys();
  _display.render();

  if (_delay_timer > 0) {
    --_delay_timer;
  }

  if (_sound_timer > 0) {
    if (_sound_timer == 1) {
      _sound.play();
    }

    --_sound_timer;
  }

  _timers_timer.restart();
}

// CLS - clear display [0x00E0]
void cpu::cls() {
  _display.clear();

  _pc += 2;
}

// RET - return from a subroutine [0x00EE]
void cpu::ret() {
  --_sp;
  _pc = _stack[_sp];

  _pc += 2;
}

// JP addr - jump to location nnn [0x1nnn]
void cpu::jmp_addr(opcode_t opcode) {
  _pc = opcode.nnn;
}

// CALL addr - call subroutine at nnn [0x2nnn]
void cpu::call_addr(opcode_t opcode) {
  _stack[_sp] = _pc;
  ++_sp;
  _pc = opcode.nnn;
}

// SE Vx, byte - skip next instruction if Vx = kk [0x3xkk]
void cpu::se_Vx_byte(opcode_t opcode) {
  if (_V[opcode.x] == opcode.kk) {
    _pc += 2;
  }

  _pc += 2;
}

// SNE Vx, byte - skip next instruction if Vx != kk [0x4xkk]
void cpu::sne_Vx_byte(opcode_t opcode) {
  if (_V[opcode.x] != opcode.kk) {
    _pc += 2;
  }

  _pc += 2;
}

// SE Vx, Vy - skip next instruction if Vx = Vy [0x5xy0]
void cpu::se_Vx_Vy(opcode_t opcode) {
  if (_V[opcode.x] == _V[opcode.y]) {
    _pc += 2;
  }

  _pc += 2;
}

// LD Vx, byte - set Vx = kk [0x6xkk]
void cpu::ld_Vx_byte(opcode_t opcode) {
  _V[opcode.x] = opcode.kk;

  _pc += 2;
}

// ADD Vx, byte - set Vx = Vx + kk [0x7xkk]
void cpu::add_Vx_byte(opcode_t opcode) {
  _V[opcode.x] += opcode.kk;

  _pc += 2;
}

// LD Vx, Vy - set Vx = Vy [0x8xy0]
void cpu::ld_Vx_Vy(opcode_t opcode) {
  _V[opcode.x] = _V[opcode.y];

  _pc += 2;
}

// OR Vx, Vy - set Vx = Vx OR Vy [0x8xy1]
void cpu::or_Vx_Vy(opcode_t opcode) {
  _V[opcode.x] |= _V[opcode.y];

  _pc += 2;
}

// AND Vx, Vy - set Vx = Vx AND Vy [0x8xy2]
void cpu::and_Vx_Vy(opcode_t opcode) {
  _V[opcode.x] &= _V[opcode.y];

  _pc += 2;
}

// XOR Vx, Vy - set Vx = Vx XOR Vy [0x8xy3]
void cpu::xor_Vx_Vy(opcode_t opcode) {
  _V[opcode.x] ^= _V[opcode.y];

  _pc += 2;
}

// ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry [0x8xy4]
void cpu::add_Vx_Vy(opcode_t opcode) {
  _V[0xF] = _V[opcode.y] > (0xFF - _V[opcode.x]);
  _V[opcode.x] += _V[opcode.y];

  _pc += 2;
}

// SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow [0x8xy5]
void cpu::sub_Vx_Vy(opcode_t opcode) {
  _V[0xF] = _V[opcode.x] >= _V[opcode.y];
  _V[opcode.x] -= _V[opcode.y];

  _pc += 2;
}

// SHR Vx {,Vy} - set Vx = Vx SHR 1 [0x8xy6]
void cpu::shr_Vx(opcode_t opcode) {
  _V[0xF] = _V[opcode.x] & 0x1;
  _V[opcode.x] >>= 1;

  _pc += 2;
}

// SUBN Vx, Vy - set Vx = Vy - Vx, set VF = NOT borrow [0x8xy7]
void cpu::subn_Vx_Vy(opcode_t opcode) {
  _V[0xF] = _V[opcode.y] >= _V[opcode.x];
  _V[opcode.x] = _V[opcode.y] - _V[opcode.x];

  _pc += 2;
}

// SHL Vx {,Vy} - set Vx = Vx SHL 1 [0x8xyE]
void cpu::shl_Vx(opcode_t opcode) {
  _V[0xF] = _V[opcode.x] >> 7;
  _V[opcode.x] <<= 1;

  _pc += 2;
}

// SNE Vx, Vy - skip next instruction if Vx != Vy [0x9xy0]
void cpu::sne_Vx_Vy(opcode_t opcode) {
  if (_V[opcode.x] != _V[opcode.y]) {
    _pc += 2;
  }

  _pc += 2;
}

// LD I, addr - set I = nnn [0xAnnn]
void cpu::ld_I_addr(opcode_t opcode) {
  _I = opcode.nnn;

  _pc += 2;
}

// JP V0, addr - jump to location nnn + V0 [0xBnnn]
void cpu::jmp_V0_addr(opcode_t opcode) {
  _pc = opcode.nnn + _V[0x0];
}

// RND Vx, byte - set Vx = random byte AND kk [0xCxkk]
void cpu::rnd_Vx_byte(opcode_t opcode) {
  _V[opcode.x] = opcode.kk & _rng.gen();

  _pc += 2;
}

// DRW Vx, Vy, nibble - display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision. [0xDxyn]
void cpu::drw_Vx_Vy_nibble(opcode_t opcode) {
  uint8_t height = opcode.n;

  for (uint8_t i = 0; i < height; ++i) {
    uint8_t pixel = _memory[_I + i];
    uint8_t y = (_V[opcode.y] + i) % ct::screen_height;

    for (uint8_t j = 0; j < 8; ++j) {
      uint8_t x = (_V[opcode.x] + j) % ct::screen_width;

      if ((pixel & (0x80 >> j)) != 0) {
        auto current_pixel = _display.get_pixel(x, y);
        _V[0xF] = current_pixel == sf::Color::White;

        if (current_pixel == sf::Color::White) {
          _display.set_pixel(x, y, sf::Color::Black);
        } else {
          _display.set_pixel(x, y, sf::Color::White);
        }
      }
    }
  }

  _pc += 2;
}

// SKP Vx - skip next instruction if key with the value of Vx is pressed [0xEx9E]
void cpu::skp_Vx(opcode_t opcode) {
  if (_keys[_V[opcode.x]]) {
    _pc += 2;
  }

  _pc += 2;
}

// SKNP Vx - skip next instruction if key with the value of Vx is not pressed [0xExA1]
void cpu::sknp_Vx(opcode_t opcode) {
  if (!_keys[_V[opcode.x]]) {
    _pc += 2;
  }

  _pc += 2;
}

// LD Vx, DT - set Vx = delay timer value [0xFx07]
void cpu::ld_Vx_DT(opcode_t opcode) {
  _V[opcode.x] = _delay_timer;

  _pc += 2;
}

// LD Vx, K - wait for a key press, store the value of the key in Vx [0xFx0A]
void cpu::ld_Vx_K(opcode_t opcode) {
  for (uint8_t i = 0; i < ct::keys_size; ++i) {
    if (_keys[i]) {
      _V[opcode.x] = i;
      _pc += 2;
      return;
    }
  }
}

// LD DT, Vx - set delay timer = Vx [0xFx15]
void cpu::ld_DT_Vx(opcode_t opcode) {
  _delay_timer = _V[opcode.x];

  _pc += 2;
}

// LD ST, Vx - set sound timer = Vx [0xFx18]
void cpu::ld_ST_Vx(opcode_t opcode) {
  _sound_timer = _V[opcode.x];

  _pc += 2;
}

// ADD I, Vx - set I = I + Vx [0xFx1E]
void cpu::add_I_Vx(opcode_t opcode) {
  // _V[0xF] = (_I + _V[opcode.x] > 0xFFF);
  _I += _V[opcode.x];

  _pc += 2;
}

// LD F, Vx - set I = location of sprite for digit Vx [0xFx29]
void cpu::ld_F_Vx(opcode_t opcode) {
  _I = _V[opcode.x] * ct::char_size;

  _pc += 2;
}

// LD B, Vx - store BCD representation of Vx in memory locations I, I + 1, and I + 2 [0xFx33]
void cpu::ld_B_Vx(opcode_t opcode) {
  _memory[_I] = _V[opcode.x] / 100;
  _memory[_I + 1] = (_V[opcode.x] / 10) % 10;
  _memory[_I + 2] = _V[opcode.x] % 10;
  // _memory[_I + 2] = (_V[opcode.x] % 100) % 10;

  _pc += 2;
}

// LD [I], Vx - store registers V0 through Vx in memory starting at location I [0xFx55]
void cpu::ld_I_Vx(opcode_t opcode) {
  for (int i = 0; i <= opcode.x; ++i) {
    _memory[_I + i] = _V[i];
  }

  _pc += 2;
}

// LD Vx, [I] - read registers V0 through Vx from memory starting at location I [0xFx65]
void cpu::ld_Vx_I(opcode_t opcode) {
  for (int i = 0; i <= opcode.x; ++i) {
    _V[i] = _memory[_I + i];
  }

  _pc += 2;
}

void cpu::opcode_0x0(opcode_t opcode) {
  switch (opcode.kk) {
    case 0xE0:
      cls();
      break;
    case 0xEE:
      ret();
      break;
    default:
      print_error("[opcode not found]", opcode.raw);
      break;
  }
}

void cpu::opcode_0x8(opcode_t opcode) {
  switch (opcode.n) {
    case 0x00:
      ld_Vx_Vy(opcode);
      break;
    case 0x01:
      or_Vx_Vy(opcode);
      break;
    case 0x02:
      and_Vx_Vy(opcode);
      break;
    case 0x03:
      xor_Vx_Vy(opcode);
      break;
    case 0x04:
      add_Vx_Vy(opcode);
      break;
    case 0x05:
      sub_Vx_Vy(opcode);
      break;
    case 0x06:
      shr_Vx(opcode);
      break;
    case 0x07:
      subn_Vx_Vy(opcode);
      break;
    case 0x0E:
      shl_Vx(opcode);
      break;
    default:
      print_error("[opcode not found]", opcode.raw);
      break;
  }
}

void cpu::opcode_0xE(opcode_t opcode) {
  switch (opcode.kk) {
    case 0x9E:
      skp_Vx(opcode);
      break;
    case 0xA1:
      sknp_Vx(opcode);
      break;
    default:
      print_error("[opcode not found]", opcode.raw);
      break;
  }
}

void cpu::opcode_0xF(opcode_t opcode) {
  switch (opcode.kk) {
    case 0x07:
      ld_Vx_DT(opcode);
      break;
    case 0x0A:
      ld_Vx_K(opcode);
      break;
    case 0x15:
      ld_DT_Vx(opcode);
      break;
    case 0x18:
      ld_ST_Vx(opcode);
      break;
    case 0x1E:
      add_I_Vx(opcode);
      break;
    case 0x29:
      ld_F_Vx(opcode);
      break;
    case 0x33:
      ld_B_Vx(opcode);
      break;
    case 0x55:
      ld_I_Vx(opcode);
      break;
    case 0x65:
      ld_Vx_I(opcode);
      break;
    default:
      print_error("[opcode not found]", opcode.raw);
      break;
  }
}
}  // namespace chip8