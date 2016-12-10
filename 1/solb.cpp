
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

struct Point {
  bool operator==(const Point &rhs) const {
    return
        this->x == rhs.x &&
        this->y == rhs.y;
  }

  int x;
  int y;
};

struct Navigation {
  Navigation() : pos({0,0}), direction(NORTH) {}

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
    pos.x += dx;
    pos.y += dy;
    std::cout << "Walking " << steps << " steps. " << position_string() << std::endl;
  }

  std::string position_string() const {
    std::ostringstream oss;
    oss << "(" << pos.x << ", " << pos.y << ")";
    return oss.str();
  }

  int x;
  int y;
  Point pos;
  Direction direction;
};

int headquarters_distance(std::fstream &fs) {
  Navigation nav;
  std::vector<Point> saved_positions;

  std::string word;
  while (fs >> word) {
    const char direction = word[0];
    assert(direction == 'L' || direction == 'R');
    nav.turn(direction == 'L' ? LEFT : RIGHT);

    const auto end_index = word.find_first_not_of("1234567890", 1);
    const auto number_string = end_index != std::string::npos ?
        word.substr(1, end_index) :
        word.substr(1);
    int steps = std::stoi(number_string);
    while (steps-- > 0) {
      nav.walk(1);

      const auto it = std::find(saved_positions.begin(), saved_positions.end(), nav.pos);
      if (it != saved_positions.end()) {
        std::cout << "We've been here before! This is Easter Bunny HQ." << std::endl;
        return std::abs(nav.pos.x) + std::abs(nav.pos.y);
      }
      saved_positions.push_back(nav.pos);
    }
  }
  return -1;
}

int main() {
  std::fstream fs;
  fs.open("input", std::fstream::in);
  
  const auto distance = headquarters_distance(fs);
  if (distance >= 0) {
    std::cout << "Total distance: " << distance << std::endl;
  } else {
    std::cout << "Unable to find Easter Bunny HQ :'-(" << std::endl;
  }

  fs.close();

  return 0;
}
