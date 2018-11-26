#include "ui.h"

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

std::string select_rom() {
  std::string buffer;
  buffer.resize(300);

#ifdef _WIN32
  OPENFILENAMEA ofn;

  ofn.lStructSize = sizeof(OPENFILENAMEA);
  ofn.hwndOwner = NULL;
  ofn.hInstance = NULL;
  ofn.lpstrFilter = NULL;
  ofn.lpstrCustomFilter = NULL;
  ofn.nMaxCustFilter = 0;
  ofn.nFilterIndex = 0;
  ofn.lpstrFile = buffer.data();
  ofn.nMaxFile = 300;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = "..\\roms\\";
  ofn.lpstrTitle = NULL;
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
    buffer = "";
  }
#else
  buffer = "";
#endif

  return buffer;
}