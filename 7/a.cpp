
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool has_support(const std::string &line) {
  bool in_hyper_seq = false;
  bool found = false;
  for (int i = 0; i < line.size() - 3; ++i) {
    const auto segment = line.substr(i, 4);
    const bool abba =
        std::all_of(segment.begin(), segment.end(), &isalpha) &&
        segment[0] == segment[3] &&
        segment[1] == segment[2] &&
        segment[0] != segment[1];  // The interior characters must be different
    if (abba) {
      if (in_hyper_seq) {
        return false;
      } else {
        found = true;
      }
    }

    const bool has_opening_bracket = segment.find('[') != std::string::npos;
    const bool has_closing_bracket = segment.find(']') != std::string::npos;
    if (has_opening_bracket) {
      in_hyper_seq = true;
    } else if (has_closing_bracket) {
      in_hyper_seq = false;
    }
  }
  return found;
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  std::string line;
  int count = 0;
  while (std::getline(ifs, line)) {
//    std::cout << (has_support(line) ? "O" : "X") << "\t" << line << std::endl;
    if (has_support(line)) {
      ++count;
    }
  }

  std::cout << "Valid IPs: " << count << std::endl;

  ifs.close();

  return 0;
}
