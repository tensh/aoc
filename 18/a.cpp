
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

std::vector<std::string> get_trap_map(const std::string &first_row, int rows) {
  std::vector<std::string> result;
  result.push_back(first_row);

  for (int row_index = 1; row_index < rows; ++row_index) {
    const std::string &prev_row(result[row_index - 1]);
    std::ostringstream oss;
    
    for (int i = 0; i < prev_row.size(); ++i) {
      oss << (is_trap(i, prev_row) ? TRAP : SAFE);
    }

    result.push_back(oss.str());
  }

  return result;
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  std::string line;
  std::getline(ifs, line);

  int safe_tiles = 0;
  const auto trap_map = get_trap_map(line, 40);
  std::for_each(trap_map.begin(), trap_map.end(), [&safe_tiles] (const std::string &row) {
    std::cout << row << std::endl;

    safe_tiles += std::count_if(row.begin(), row.end(), [] (char c) {
      return c == SAFE;
    });
  });

  std::cout << "Safe tiles: " << safe_tiles << std::endl;

  ifs.close();

  return 0;
}
