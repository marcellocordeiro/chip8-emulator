#pragma once

#include <filesystem>

namespace lib {
void                  hide_console();
void                  show_console();
void                  message_box(const char* message);
std::filesystem::path select_rom();
}  // namespace lib
