#include <filesystem>

#include "cpu.h"
#include "display.h"
#include "ini.h"
#include "input.h"
#include "sound.h"
#include "timer.h"
#include "ui.h"

int main() {
  hide_console();
  chip8::display display;
  chip8::sound sound;
  chip8::input keys;

  chip8::cpu cpu(display, sound, keys);
  cpu.load(select_rom());

  bool running = true;

  try {
    while (display.is_open()) {
      switch (display.poll_events()) {
        case ac::none:
          break;
        case ac::pause:
          running = !running;
          break;
        case ac::reset:
          cpu.reset();
          cpu.load(select_rom());
          break;
      }

      if (running) {
        cpu.cycle();
        cpu.cycle_timers();
      }
    }
  } catch (std::exception& e) {
    display.close();
    message_box(e.what());
  }

  return 0;
}