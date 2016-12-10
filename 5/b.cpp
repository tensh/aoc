
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "md5.h"

std::string find_code(const std::string &puzzle) {
  int found = 0;

  std::string code;
  code.resize(8);
  for (int seq = 1; found != 0xFF; seq++) {
    std::ostringstream oss;
    oss << puzzle << seq;
    const auto &hashed = md5(oss.str());
    if (hashed.find("00000") == 0) {
      const int pos = hashed[5] - '0';
      if (pos >= 8) {
        continue;
      }

      const char c = hashed[6];
      if ((found & (1 << pos)) == 0) {
        std::cout << "Hit: " << oss.str() << " => " << c << "@" << pos << " (" << hashed << ")" << std::endl;
        code[pos] = c;
        found |= 1 << pos;
      } else {
        std::cout << "Ignored: " << oss.str() << " => " << c << "@" << pos << " (" << hashed << ")" << std::endl;
      }
    }
  }
  return code;
}

int main() {
  const std::string puzzle("wtnhxymk");

  const auto &code = find_code(puzzle);
  std::cout << "Code is " << code << std::endl;

  return 0;
}
