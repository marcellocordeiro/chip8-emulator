#include "input.h"

void update_keys(std::array<bool, cte::keys_size>& keys) {
  keys[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1);
  keys[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2);
  keys[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3);
  keys[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4);

  keys[4] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
  keys[5] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
  keys[6] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
  keys[7] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);

  keys[8] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
  keys[9] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
  keys[10] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
  keys[11] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);

  keys[12] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z);
  keys[13] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);
  keys[14] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C);
  keys[15] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V);
}