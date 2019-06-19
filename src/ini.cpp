#include "ini.h"

void trim_string(std::string& str)
{
  str.erase(
      std::remove_if(
          str.begin(), str.end(), [](char x) { return std::isspace(x); }),
      str.end());
}

ini::ini(const std::string& file_name)
{
  std::ifstream file(file_name);
  std::string   name, value;

  while (std::getline(file, name, '=')) {
    std::getline(file, value);
    trim_string(name);
    trim_string(value);
    settings[name] = value;
  }

  /* for (const auto& [name_cur, value_cur] : settings) {
    std::cout << name_cur << ' ' << value_cur << std::endl;
  } */
}

const std::string& ini::operator[](const std::string& str)
{
  return settings[str];
}