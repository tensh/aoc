
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using KeyRow = std::array<int, 5>;
using KeyPad = std::array<KeyRow, 5>;

class Navigation {
 public:
  Navigation() : x(0), y(2) {}  // Start at '5'

  void move(char dir) {
    const int dx = dir == 'L' ? -1 : dir == 'R' ? 1 : 0;
    const int dy = dir == 'D' ? 1 : dir == 'U' ? -1 : 0;  // Reverse order
    const int new_x = std::max(0, std::min((int)(keypad.size() - 1), x + dx));
    const int new_y = std::max(0, std::min((int)(keypad.size() - 1), y + dy));

    if (keypad[new_y][new_x] != 0) {
      x = new_x;
      y = new_y;
    }
  }

  char find_digit(const std::string &line) {
    std::for_each(line.begin(), line.end(), [this] (char c) {
      move(c);
    });
    return digit();
  }

 private:
  char digit() const {
    std::ostringstream oss;
    oss << std::hex << std::uppercase << keypad[y][x];
    return oss.str()[0];
  }

  // Keypad:
  //     1
  //   2 3 4
  // 5 6 7 8 9
  //   A B C
  //     D
  const KeyPad keypad = {
    KeyRow { 0,   0,   1,   0, 0 },
    KeyRow { 0,   2,   3,   4, 0 },
    KeyRow { 5,   6,   7,   8, 9 },
    KeyRow { 0, 0xa, 0xb, 0xc, 0 },
    KeyRow { 0,   0, 0xd,   0, 0 }};

  int x;
  int y;
};



int main() {
  std::ifstream ifs;
  ifs.open("input");

  Navigation nav;

  std::string line; 
  while (std::getline(ifs, line)) {
    std::cout << nav.find_digit(line);
  }
  std::cout << std::endl;

  ifs.close();

  return 0;
}
