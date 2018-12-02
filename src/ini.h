#pragma once

#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class ini {
public:
  ini(const std::string& file_name);

  const std::string& operator[](const std::string&);

private:
  std::unordered_map<std::string, std::string> settings;
};