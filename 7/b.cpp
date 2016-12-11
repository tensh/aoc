
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Segments = std::vector<std::string>;

Segments invert_segments(const Segments &segs) {
  Segments inverted;
  std::transform(segs.begin(), segs.end(), std::back_inserter(inverted),
      [] (const std::string &s) {
    std::ostringstream oss;
    oss << s[1] << s[0] << s[1];
    return oss.str();
  });
  return inverted;
}

bool has_support(const std::string &line) {
  Segments external;
  Segments internal;

  const int segment_length = 3;
  bool in_hyper_seq = false;
  for (int i = 0; i < line.size() - segment_length + 1; ++i) {
    const auto segment = line.substr(i, segment_length);
    const bool abba =
        std::all_of(segment.begin(), segment.end(), &isalpha) &&
        segment[0] == segment[2] &&
        segment[0] != segment[1];  // The interior characters must be different
    if (abba) {
      if (in_hyper_seq) {
        internal.push_back(segment);
      } else {
        external.push_back(segment);
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

  const auto &inverted_segments = invert_segments(external);

  return std::any_of(internal.begin(), internal.end(), [&inverted_segments] (const std::string &segment) {
    return std::find(inverted_segments.begin(), inverted_segments.end(), segment) != inverted_segments.end();
  });
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  std::string line;
  int count = 0;
  while (std::getline(ifs, line)) {
    //std::cout << (has_support(line) ? "O" : "X") << "\t" << line << std::endl;
    if (has_support(line)) {
      ++count;
    }
  }

  std::cout << "Valid IPs: " << count << std::endl;

  ifs.close();

  return 0;
}
