#pragma once

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

void hide_console();
void show_console();
void message_box(const char* message);
std::string select_rom();