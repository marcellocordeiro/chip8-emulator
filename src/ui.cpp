#include "ui.h"

namespace chip8 {
void message_box(const char* message) {
#ifdef _WIN32
  MessageBox(NULL, message, "Error!", MB_OK);
#endif

#ifdef __linux__
  std::string command = "gdialog --title \"Error!\" --msgbox \"";
  command += std::string(message);
  command += "\"";
  system(command.c_str());
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
  char           file_path[buffer_size];

#ifdef _WIN32
  const char dialog_title[] = "Select a ROM";
  const char initial_dir[]  = "..\\roms\\";

  OPENFILENAMEA ofn;
  ZeroMemory(&ofn, sizeof(ofn));

  ofn.lStructSize     = sizeof(ofn);
  ofn.hwndOwner       = nullptr;
  ofn.lpstrFile       = file_path;
  ofn.lpstrFile[0]    = '\0';
  ofn.nMaxFile        = sizeof(file_path);
  ofn.lpstrFilter     = "All\0*.*\0";
  ofn.nFilterIndex    = 1;
  ofn.lpstrFileTitle  = nullptr;
  ofn.nMaxFileTitle   = 0;
  ofn.lpstrInitialDir = initial_dir;
  ofn.lpstrTitle      = dialog_title;
  ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (!GetOpenFileNameA(&ofn)) {
    throw std::runtime_error("Couldn't select a ROM");
  }

  return std::filesystem::canonical(std::filesystem::path(file_path));
#endif

#ifdef __linux__
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("zenity --file-selection", "r"), pclose);

  if (!pipe) {
    throw std::runtime_error("popen() failed");
  }

  fgets(file_path, buffer_size, pipe.get());

  {
    std::size_t i = 0;
    while (file_path[i] != '\0') {
      ++i;

      if (file_path[i] == '\n') {
        file_path[i] = '\0';
      }
    }
  }

  if (file_path[0] == '\0') {
    throw std::runtime_error("Couldn't select a ROM");
  }

  return std::filesystem::canonical(std::filesystem::path(file_path));
#endif

  return std::filesystem::current_path().parent_path() / "roms" / "BRIX";
}
}  // namespace chip8