
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

enum State {
  cStateText,
  cStateCharReads,
  cStateCharTimes
};

size_t decompressed_text_length(const std::string &line);

size_t repeat_text_length(const std::string &repeat, int times) {
  return decompressed_text_length(repeat) * times;
}

size_t decompressed_text_length(const std::string &line) {
  std::ostringstream oss;
  std::ostringstream char_reads;
  std::ostringstream char_times;

  size_t sum = 0;

  State state = cStateText;
  for (int i = 0; i < line.size(); ++i) {
    char c = line[i];
    switch (state) {
      case cStateText: {
        if (c == '(') {
          state = cStateCharReads;
        } else {
          oss << c;
          ++sum;
        }
        break;
      }
      case cStateCharReads: {
        if (c == 'x') state = cStateCharTimes;
        else char_reads << c;
        break;
      }
      case cStateCharTimes: {
        if (c == ')') {
          state = cStateText;
          const int reads = stoi(char_reads.str());
          sum += repeat_text_length(line.substr(i + 1, reads), stoi(char_times.str()));
          i += reads;

          // Clean up buffers
          char_reads.str("");
          char_reads.clear();
          char_times.str("");
          char_times.clear();
        } else {
          char_times << c;
        }
        break;
      }
    }
  }

  return sum;
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  size_t total_length = 0;
  std::string line;
  while (std::getline(ifs, line)) {
    total_length += decompressed_text_length(line);
  }

  std::cout << "Total decompressed length: " << total_length << std::endl;

  ifs.close();

  return 0;
}
