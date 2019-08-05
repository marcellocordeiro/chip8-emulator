#include "emulator.h"
#include "system_utils.h"

int main()
{
  lib::hide_console();

  try {
    chip8::emulator emulator;

    emulator.power_on();
    emulator.run();
  } catch (const std::exception& e) {
    lib::message_box(e.what());
  }

  return 0;
}
