
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


const char *INPUT = "11100010111110100";
//const char *INPUT = "10000";
//const size_t TARGET_LEN = 20;

// Exercise a
//const size_t TARGET_LEN = 272;
// Exercise b
const size_t TARGET_LEN = 35651584;

std::string apply_dragon_curve(const std::string &input, size_t target_len) {
  auto a(input);
  while (a.size() < target_len) {
    const auto b = std::string(a.rbegin(), a.rend());
    std::string res;
    std::transform(b.begin(), b.end(), std::back_inserter(res), [] (const char c) {
      return c == '0' ? '1' : '0';
    });
    a += '0' + res;
  }
  return a;
}

std::string generate_checksum(const std::string &input) {
  std::string checksum(input);
  do {
    std::ostringstream oss;
    for (int i = 0; i < checksum.size() / 2; ++i) {
      const char a = checksum[2 * i];
      const char b = checksum[2 * i + 1];

      oss << (a == b ? '1' : '0');
    }
    checksum = oss.str();
  } while ((checksum.size() % 2) == 0);
  return checksum;
}

int main() {
  const auto dc = apply_dragon_curve(INPUT, TARGET_LEN);
  std::cout << "DC: " << dc << std::endl;
  std::cout << "Checksum: " << generate_checksum(dc.substr(0, TARGET_LEN)) << std::endl;

  return 0;
}
