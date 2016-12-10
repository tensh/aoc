
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int valid_triangles(std::ifstream &ifs) {
  int a, b, c;

  int result = 0;

  while (ifs >> a) {
    if (!(ifs >> b)) throw "Invalid input.";
    if (!(ifs >> c)) throw "Invalid input.";

    std::vector<int> values = { a, b, c };
    std::sort(values.begin(), values.end());

    if ((values[0] + values[1]) > values[2]) {
      ++result;
    }
  }

  return result;
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  std::cout << "Valid triangles: " << valid_triangles(ifs) << std::endl;

  ifs.close();

  return 0;
}
