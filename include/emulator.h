#pragma once

#include <memory>

#include "types/forward_decl.h"

namespace chip8 {
class emulator {
public:
  emulator();
  ~emulator();

  void power_on();
  void run();

  chip8::cpu*   get_cpu();
  chip8::audio* get_audio();
  chip8::input* get_input();
  chip8::io*    get_io();

private:
  std::unique_ptr<chip8::cpu>   cpu;
  std::unique_ptr<chip8::audio> audio;
  std::unique_ptr<chip8::input> input;
  std::unique_ptr<chip8::io>    io;
};
}  // namespace chip8
