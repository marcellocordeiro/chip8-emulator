#pragma once

#include <filesystem>

namespace chip8 {
void                  hide_console();
void                  show_console();
void                  message_box(const char* message);
std::filesystem::path select_rom();
}  // namespace chip8
