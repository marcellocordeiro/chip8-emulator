#include "emulator.h"

#include "audio.h"
#include "cpu.h"
#include "input.h"
#include "io.h"
#include "system_utils.h"

namespace chip8 {
emulator::emulator()
  : cpu(std::make_unique<chip8::cpu>(*this)),
    audio(std::make_unique<chip8::audio>()),
    input(std::make_unique<chip8::input>()),
    io(std::make_unique<chip8::io>(*this))
{}

emulator::~emulator() = default;

void emulator::power_on()
{
  cpu->power_on();
  cpu->load(lib::select_rom());
}

void emulator::run()
{
  io->run();
}

chip8::cpu* emulator::get_cpu()
{
  return cpu.get();
}

chip8::audio* emulator::get_audio()
{
  return audio.get();
}

chip8::input* emulator::get_input()
{
  return input.get();
}

chip8::io* emulator::get_io()
{
  return io.get();
}
}  // namespace chip8
