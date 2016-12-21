
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

size_t naive_solution(size_t num_elves) {
  std::vector<size_t> elves;
  for (size_t i = 1; i <= num_elves; ++i) {
    elves.push_back(i);
  }

  size_t current = 0;
  while (elves.size() > 1) {
    const size_t victim = (current + (size_t)(elves.size() / 2)) % elves.size();
    //std::cout << elves[current] << " -> " << elves[victim] << std::endl;
    elves.erase(elves.begin() + victim);
    current = (current < victim ? current + 1 : current) % elves.size();
  }

  return elves[0];
}


/* The sequence for the first 100 numbers, starting at 2 looks like this:
 *
 *  1       1     |  *  51      24
 *  2       1     |  *  52      25
 *  3       3     |  *  53      26
 *  4       1     |  *  54      27
 *  5       2     |  *  55      29
 *  6       3     |  *  56      31
 *  7       5     |  *  57      33
 *  8       7     |  *  58      35
 *  9       9     |  *  59      37
 *  10      1     |  *  60      39
 *  11      2     |  *  61      41
 *  12      3     |  *  62      43
 *  13      4     |  *  63      45
 *  14      5     |  *  64      47
 *  15      6     |  *  65      49
 *  16      7     |  *  66      51
 *  17      8     |  *  67      53
 *  18      9     |  *  68      55
 *  19      11    |  *  69      57
 *  20      13    |  *  70      59
 *  21      15    |  *  71      61
 *  22      17    |  *  72      63
 *  23      19    |  *  73      65
 *  24      21    |  *  74      67
 *  25      23    |  *  75      69
 *  26      25    |  *  76      71
 *  27      27    |  *  77      73
 *  28      1     |  *  78      75
 *  29      2     |  *  79      77
 *  30      3     |  *  80      79
 *  31      4     |  *  81      81
 *  32      5     |  *  82      1
 *  33      6     |  *  83      2
 *  34      7     |  *  84      3
 *  35      8     |  *  85      4
 *  36      9     |  *  86      5
 *  37      10    |  *  87      6
 *  38      11    |  *  88      7
 *  39      12    |  *  89      8
 *  40      13    |  *  90      9
 *  41      14    |  *  91      10
 *  42      15    |  *  92      11
 *  43      16    |  *  93      12
 *  44      17    |  *  94      13
 *  45      18    |  *  95      14
 *  46      19    |  *  96      15
 *  47      20    |  *  97      16
 *  48      21    |  *  98      17
 *  49      22    |  *  99      18
 *  50      23    |  *  100     19
 *
 * It seems like we only need to look at x - 3^n for the biggest n that results
 * in a positive number. Perfect powers of 3 will always return themselves, and
 * after that counting starts over. The winning number increases by one until
 * we're halfway to the next power of three, after which it increases by two
 * per number.
 *
 */

size_t analytical_solution(size_t num_elves) {
  const auto n = static_cast<size_t>(std::log(num_elves) / std::log(3));
  const auto hv = static_cast<size_t>(std::pow(3, n));
  const auto rest = num_elves - hv;

  // Special case for numbers that are powers of 3
  if (rest == 0) {
    return num_elves;
  }

  // In the range (3^n, 3^(n+1)], the first half will increase one by
  // one. After that, it will always increase by two.
  const auto span = static_cast<size_t>(std::pow(3, n + 1)) - hv;
  const auto half_span = static_cast<size_t>(span / 2);

  const auto extra = rest > half_span ? rest - half_span : 0UL;

  return rest + extra;
}

void test_solution() {
  for (size_t i = 1; i <= 8000; ++i) {
    const auto a = analytical_solution(i);
    const auto n = naive_solution(i);
    std::cout << "Testing " << i << "... ";
    if (a == n) {
      std::cout << "OK" << std::endl;
    } else {
      std::cout << "FAIL! (" << a << " != " << n << ")" << std::endl;
      assert(false);
    }
  }
}

int main() {
  const size_t input = 3005290;

  //test_solution();
  std::cout << analytical_solution(input) << std::endl;

  return 0;
}
