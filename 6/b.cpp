
#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using CharMap = std::map<char, int>;
using Columns = std::array<CharMap, 8>;

void add_character(CharMap &map, char c) {
  if (map.count(c) == 0) {
    map[c] = 1;
  } else {
    ++map[c];
  }
}

void process_row(Columns &columns, const std::string &line) {
  assert(columns.size() == line.size());

  for (int i = 0; i < line.size(); ++i) {
    add_character(columns[i], line[i]);
  }
}

char least_common_char(const CharMap &map) {
  using Pair = std::pair<char, int>;
  const auto it = std::min_element(map.begin(), map.end(), [] (const Pair &p1, const Pair &p2) {
    return p1.second < p2.second;
  });
  return it->first;
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  Columns cols;

  std::string line;
  while (std::getline(ifs, line)) {
    process_row(cols, line);
  }

  std::string decoded;
  std::transform(cols.begin(), cols.end(), std::back_inserter(decoded), &most_common_char);

  std::cout << "Decoded string: " << decoded << std::endl;

  ifs.close();

  return 0;
}
