#pragma once

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <string>
#endif

namespace chip8 {
void hide_console();
void show_console();
void message_box(const char* message);
std::filesystem::path select_rom();
}  // namespace chip8