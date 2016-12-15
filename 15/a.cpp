
#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using List = std::array<int, 6>;

int find_drop_point(const List &position_counts, List &positions) {
  for (int i = 1; ; ++i) {
    //std::cout << (i - 2) << ".\t";
    bool found = true;
    for (int j = 0; j < positions.size(); ++j) {
      const auto pos = positions[j];
      const auto pos_count = position_counts[j];

      const auto future_pos = (pos + j) % pos_count;
      //std::cout << future_pos << " ";
      found &= future_pos == 0;

      positions[j] = (pos + 1) % pos_count;
    }
    //std::cout << std::endl;
    if (found) {
      // Time starts at 0 (reduces index by 1)
      // It takes 1s for capsule to reach first disc (reduces index by 1)
      return i - 2;
    }
  }
  return -1;
}

int main() {
  const List position_counts = { 17, 3, 19, 13, 7, 5 };
  List positions = { 15, 2, 4, 2, 2, 0 };

  const auto drop_point = find_drop_point(position_counts, positions);
  std::cout << "1. Press button" << std::endl;
  std::cout << "2. Pray" << std::endl;
  std::cout << "3. ???" << std::endl;
  std::cout << "4. Success! (" << drop_point << ")" << std::endl;

  return 0;
}
