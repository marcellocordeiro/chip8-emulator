#include "ui.h"

void message_box(const char* message) {
#ifdef _WIN32
  MessageBox(NULL, message, "Error!", MB_OK);
#endif

#ifdef __linux__
  // todo
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
  ZeroMemory(&ofn, sizeof(ofn));

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = nullptr;
  ofn.lpstrFile = file_path;
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(file_path);
  ofn.lpstrFilter = "All\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = nullptr;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = initial_dir;
  ofn.lpstrTitle = dialog_title;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (!GetOpenFileNameA(&ofn)) {
    throw std::runtime_error("Can't select a ROM");
  }
#else
  return std::filesystem::current_path().parent_path() / "roms" / "BRIX";
#endif

  return std::filesystem::canonical(std::filesystem::path(file_path));
}