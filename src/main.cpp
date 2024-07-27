#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.h"

int main() {
  Game g("../src/config.txt");
  g.run();
  // Vec2 v1 = Vec2(3,2);
  // Vec2 v2 = Vec2(4,1);
  // Vec2 distance = v1 + v2;
  // std::cout << distance.x << " " << distance.y;
}
