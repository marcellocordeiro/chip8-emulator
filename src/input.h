#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <array>

#include "common.hpp"

void update_keys(std::array<bool, cte::keys_size>&);

/*
class input {
public:

private:
  std::array<bool, cte::keys_size> keys;
};*/