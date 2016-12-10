
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
    std::vector<int> c1, c2, c3;

    c1.push_back(a);
    if (!(ifs >> b)) throw "Invalid input.";
    if (!(ifs >> c)) throw "Invalid input.";
    c2.push_back(b);
    c3.push_back(c);

    for (int i = 0; i < 2; ++i) {
      if (!(ifs >> a)) throw "Invalid input.";
      if (!(ifs >> b)) throw "Invalid input.";
      if (!(ifs >> c)) throw "Invalid input.";
      c1.push_back(a);
      c2.push_back(b);
      c3.push_back(c);
    }

    std::vector<std::vector<int>> cols = { c1, c2, c3 };
    for (auto &col : cols) {
      assert(col.size() == 3);
      std::sort(col.begin(), col.end());

      if ((col[0] + col[1]) > col[2]) {
        ++result;
      }
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
