#include <filesystem>

#include <iostream>
#include "audio.h"
#include "cpu.h"
#include "display.h"
#include "ini.h"
#include "input.h"
#include "timer.h"
#include "ui.h"

int main() {
  chip8::hide_console();

  chip8::display display;
  chip8::audio   sound;
  chip8::input   keys;
  chip8::cpu     cpu(display, sound, keys);

  try {
    cpu.load(chip8::select_rom());
    bool running = true;

    while (display.is_open()) {
      auto& actions = display.poll_events();

      while (!actions.empty()) {
        switch (actions.front()) {
          case action_type::pause: running = !running; break;
          case action_type::reset:
            cpu.reset();
            cpu.load(chip8::select_rom());
            break;
        }

        actions.pop();
      }

      if (running) {
        cpu.cycle();
      }
    }
  } catch (const std::exception& e) {
    display.close();
    chip8::message_box(e.what());
  }

  return 0;
}