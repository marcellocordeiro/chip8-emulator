#include "ui.h"
#include <iostream>

void message_box(const char* message) {
#ifdef _WIN32
  MessageBox(NULL, message, "Error!", MB_OK);
#endif

#ifdef __linux__
  std::cout << message << std::endl;
#endif
}

void hide_console() {
#ifdef _WIN32
  ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
}

void show_console() {
#ifdef _WIN32
  ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif
}

std::filesystem::path select_rom() {
  constexpr auto buffer_size = 300;
  char file_path[buffer_size];

  const char dialog_title[] = "Select a ROM";
  const char initial_dir[] = "..\\roms\\";

#ifdef _WIN32
  OPENFILENAMEA ofn;

  ofn.lStructSize = sizeof(OPENFILENAMEA);
  ofn.hwndOwner = NULL;
  ofn.hInstance = NULL;
  ofn.lpstrFilter = NULL;
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter = 0;
  ofn.nFilterIndex = 0;
  ofn.lpstrFile = file_path;
  ofn.nMaxFile = buffer_size;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = initial_dir;
  ofn.lpstrTitle = dialog_title;
  ofn.Flags = OFN_EXPLORER;
  ofn.nFileOffset = 0;
  ofn.nFileExtension = 0;
  ofn.lpstrDefExt = NULL;
  ofn.lCustData = NULL;
  ofn.lpfnHook = NULL;
  ofn.lpTemplateName = NULL;
#ifdef _MAC
  ofn.lpEditInfo = NULL;
  ofn.lpstrPrompt = NULL;
#endif
#if (_WIN32_WINNT >= 0x0500)
  ofn.pvReserved = 0;
  ofn.dwReserved = 0;
  ofn.FlagsEx = 0;
#endif
  if (!GetOpenFileNameA(&ofn)) {
    return std::filesystem::current_path().parent_path() / "roms" / "BRIX";
  }
#else
  return std::filesystem::current_path().parent_path() / "roms" / "BRIX";
#endif

  return std::filesystem::canonical(std::filesystem::path(file_path));
}