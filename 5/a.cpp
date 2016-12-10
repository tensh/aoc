
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "md5.h"

std::string find_code(const std::string &puzzle) {
  std::string code;
  for (int seq = 1; code.size() < 8; seq++) {
    std::ostringstream oss;
    oss << puzzle << seq;
    const auto &hashed = md5(oss.str());
    if (hashed.find("00000") == 0) {
      std::cout << "Hit: " << oss.str() << " => " << hashed[5] << " (" << hashed << ")" << std::endl;
      code.push_back(hashed[5]);
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
