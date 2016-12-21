
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

size_t naive_solution(size_t num_elves) {
  assert(num_elves > 0);
  std::vector<size_t> elves;
  for (size_t i = 1; i <= num_elves; ++i) {
    elves.push_back(i);
  }

  size_t current = 0;
  while (elves.size() > 1) {
    const size_t victim = (current + 1) % elves.size();
    elves.erase(elves.begin() + victim);
    current = (current < victim ? current + 1 : current) % elves.size();
  }

  return elves[0];
}

// [1, 2, 3, 4, 5, 6, 7, 8, 9]
// [3, 5, 7, 9]: value 2 (offset += 2 * 1)
// [3, 7]: value 4
// [3]: value 8

// [1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 13]: step 1
// [3, 5, 7, 9, 11, 13]: step 2 (offset += 2*1)
// [3, 7, 11]: step 4
// [11]: (offset += 2*4)

size_t find_elf(size_t num_elves) {
  size_t first = 1;
  size_t step = 1;
  size_t left = num_elves;

  while (left > 1) {
    const bool even = (left % 2) == 0;
    if (!even) {
      first += 2 * step;
    }
    step *= 2;
    left /= 2;
  }

  return first;
}

void test_solution() {
  for (size_t i = 1; i <= 8000; ++i) {
    const auto e = find_elf(i);
    const auto n = naive_solution(i);
    std::cout << "Testing " << i << "... ";
    if (e == n) {
      std::cout << "OK" << std::endl;
    } else {
      std::cout << "FAIL! (" << e << " != " << n << ")" << std::endl;
      assert(false);
    }
  }
}

int main() {
  const size_t input = 3005290;

  //test_solution();

  const auto winner = find_elf(input);
  std::cout << "Winner: #" << winner << std::endl;

  return 0;
}
