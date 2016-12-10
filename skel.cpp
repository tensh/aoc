
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  std::ifstream ifs;
  ifs.open("input");

  //std::string line;
  //while (std::getline(ifs, line)) {
  std::string word;
  while (ifs >> word) {
  }

  ifs.close();

  return 0;
}
