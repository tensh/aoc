
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

struct Navigation {
  void move(char dir) {
    const int dx = dir == 'L' ? -1 : dir == 'R' ? 1 : 0;
    const int dy = dir == 'D' ? -1 : dir == 'U' ? 1 : 0;
    x = std::max(0, std::min(2, x + dx));
    y = std::max(0, std::min(2, y + dy));
  }

  int digit() const {
    return
        (x + 1) +     // compensate for 0-index
        (2 - y) * 3;  // reverse order (since 1 is top left);
  }

  int x;
  int y;
};

int find_digit(const std::string &line) {
  // 1 2 3
  // 4 5 6
  // 7 8 9
  Navigation nav = { 1, 1 };  // Number 5

  std::for_each(line.begin(), line.end(), [&nav] (char c) {
    nav.move(c);
  });

  return nav.digit();
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  std::string line; 
  while (std::getline(ifs, line)) {
    std::cout << find_digit(line);
  }
  std::cout << std::endl;

  ifs.close();

  return 0;
}
