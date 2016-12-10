
#include <assert.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

enum Direction {
  WEST,
  NORTH,
  EAST,
  SOUTH
};

enum TurnDirection {
  LEFT,
  RIGHT
};

struct Navigation {
  Navigation() : x(), y(), direction(NORTH) {}

  void turn(TurnDirection dir) {
    std::cout << "Turning " << (dir == LEFT ? "left" : "right") << ". ";
    switch (direction) {
      case WEST: { direction = dir == LEFT ? SOUTH : NORTH; break; }
      case NORTH: { direction = dir == LEFT ? WEST : EAST; break; }
      case EAST: { direction = dir == LEFT ? NORTH : SOUTH; break; }
      case SOUTH: { direction = dir == LEFT ? EAST : WEST; break; }
    }
  }

  void walk(int steps) {
    int dx = direction == WEST ? -steps : direction == EAST ? steps : 0;
    int dy = direction == SOUTH ? -steps : direction == NORTH ? steps : 0;
    x += dx;
    y += dy;
    std::cout << "Walking " << steps << " steps. " << position_string() << std::endl;
  }

  std::string position_string() const {
    std::ostringstream oss;
    oss << "(" << x << ", " << y << ")";
    return oss.str();
  }

  int x;
  int y;
  Direction direction;
};

int main() {
  std::fstream fs;
  fs.open("input", std::fstream::in);

  Navigation nav;

  std::string word;
  while (fs >> word) {
    const char direction = word[0];
    assert(direction == 'L' || direction == 'R');
    nav.turn(direction == 'L' ? LEFT : RIGHT);

    const auto end_index = word.find_first_not_of("1234567890", 1);
    const auto number_string = end_index != std::string::npos ?
        word.substr(1, end_index) :
        word.substr(1);
    nav.walk(std::stoi(number_string));
  }

  const auto distance = std::abs(nav.x) + std::abs(nav.y);
  std::cout << "Total distance: " << distance << std::endl;

  fs.close();

  return 0;
}
