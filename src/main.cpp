#include <filesystem>
#include <iostream>
#include <string>

#include "common.hpp"
#include "cpu.h"
#include "display.h"
#include "input.h"
#include "sound.h"
#include "timer.h"
#include "ui.h"

int main() {
  hide_console();
  std::filesystem::path rom_file = select_rom();
  chip8::cpu cpu;
  chip8::sound sound;
  chip8::display display;
  chip8::timer cycle_timer, timers_timer;

  std::cout << rom_file << std::endl;

  if (rom_file.string() == "") {
    // todo
    std::string rom = "BRIX";
    rom_file = std::filesystem::current_path().parent_path() / "roms" / rom;
  }

  cpu.load(rom_file.string());

  try {
    while (display.is_open()) {
      switch (display.poll_events()) {
        case actions::NONE:
          break;
        case actions::RESET:
          cpu = chip8::cpu();
          cpu.load(rom_file.string());
          break;
      }

      if (cycle_timer.elapsed_time() >= 1.0f / cte::cpu_clock) {
        cpu.cycle();

        cycle_timer.restart();
      }

      if (timers_timer.elapsed_time() >= 1.0f / cte::cpu_counters_clock) {
        cpu.cycle_timers();
        cpu.update_keys();
        display.render(cpu.get_gfx());

        if (cpu.get_beep()) {
          sound.play();
        } else {
          sound.stop();
        }

        timers_timer.restart();
      }
    }
  } catch (std::exception e) {
    display.close();
    message_box(e.what());
  }

  return 0;
}