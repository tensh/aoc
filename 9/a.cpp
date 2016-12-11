
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

void repeat_text(std::ostringstream &output, const std::string &repeat, int times) {
  while (times-- > 0) {
    output << repeat;
  }
}

std::string decompress_text(const std::string &line) {
  std::ostringstream oss;
  std::ostringstream char_reads;
  std::ostringstream char_times;

  State state = cStateText;
  for (int i = 0; i < line.size(); ++i) {
    char c = line[i];
    switch (state) {
      case cStateText: {
        if (c == '(') state = cStateCharReads;
        else oss << c;
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
          repeat_text(oss, line.substr(i + 1, reads), stoi(char_times.str()));
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

  return oss.str();
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  int total_length = 0;
  std::string line;
  while (std::getline(ifs, line)) {
    total_length += decompress_text(line).size();
  }

  std::cout << "Total decompressed length: " << total_length << std::endl;

  ifs.close();

  return 0;
}
