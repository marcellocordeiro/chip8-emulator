#include <filesystem>

#include "cpu.h"
#include "display.h"
#include "ini.h"
#include "input.h"
#include "sound.h"
#include "timer.h"
#include "ui.h"

int main() {
  chip8::hide_console();

  chip8::display display;
  chip8::sound sound;
  chip8::input keys;

  chip8::cpu cpu(display, sound, keys);

  try {
    cpu.load(chip8::select_rom());
    bool running = true;

    while (display.is_open()) {
      switch (display.poll_events()) {
        case ac::none:
          break;
        case ac::pause:
          running = !running;
          break;
        case ac::reset:
          cpu.reset();
          cpu.load(chip8::select_rom());
          break;
      }

      if (running) {
        cpu.cycle();
        cpu.cycle_timers();
      }
    }
  } catch (std::exception& e) {
    display.close();
    chip8::message_box(e.what());
  }

  return 0;
}