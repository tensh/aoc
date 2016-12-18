
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const char TRAP = '^';
const char SAFE = '.';

bool is_trap(int index, const std::string &prev_row) {
  bool trap_left  = index > 0 ? prev_row[index-1] == TRAP : false;
  bool trap_mid   = prev_row[index] == '.';
  bool trap_right = index < (prev_row.size() - 1) ? prev_row[index+1] == TRAP : false;

  return
      (trap_left && trap_mid && !trap_right) ||
      (!trap_left && trap_mid && trap_right) ||
      (trap_left && !trap_mid && !trap_right) ||
      (!trap_left && !trap_mid && trap_right);
}

size_t safe_tiles_in_row(const std::string &row) {
  return std::count_if(row.begin(), row.end(), [] (char c) {
    return c == SAFE;
  });
}

size_t find_safe_tiles(const std::string &first_row, int rows) {
  std::string prev_row(first_row);
  size_t safe_tiles = safe_tiles_in_row(prev_row);
  for (int row_index = 1; row_index < rows; ++row_index) {
    std::ostringstream oss;
    
    for (int i = 0; i < prev_row.size(); ++i) {
      oss << (is_trap(i, prev_row) ? TRAP : SAFE);
    }

    prev_row = oss.str();
    safe_tiles += safe_tiles_in_row(prev_row);
  }

  return safe_tiles;
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  std::string line;
  std::getline(ifs, line);

  const auto safe_tiles = find_safe_tiles(line, 400000);
  std::cout << "Safe tiles: " << safe_tiles << std::endl;

  ifs.close();

  return 0;
}
