
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>

#include "md5.h"

const size_t KEYS_NEEDED = 64;

bool find_sequence(const std::string &line, int count, char &seq_char) {
  char prev = '\0';
  int occurrences = 0;
  for (int i = 0; i < line.size(); ++i) {
    const char c = line[i];

    if (c == prev) {
      if (++occurrences == count) {
        seq_char = c;
        return true;
      }
    } else {
      prev = c;
      occurrences = 1;
    }
  }
  return false;
}

bool is_valid_key(const std::string &salt, char seq_char, int index) {
  const std::string search_string(5, seq_char);
  for (int j = 1; j <= 1000; ++j) {
    const auto &hashed = md5(salt + std::to_string(index + j));
    if (hashed.find(search_string) != std::string::npos) {
      return true;
    }
  }
  return false;
}

void find_pad_keys(const std::string &salt) {
  int found_keys = 0;
  for (int index = 0; found_keys < KEYS_NEEDED; ++index) {
    const auto &hashed = md5(salt + std::to_string(index));

    char seq_char;
    if (find_sequence(hashed, 3, seq_char) && is_valid_key(salt, seq_char, index)) {
      std::cout << ++found_keys << "\t" << (char)seq_char << " (" << index << ")" << std::endl;
    }
  }
}

int main() {
  //const char *SALT = "abc";
  const char *SALT = "zpqevtbw";

  find_pad_keys(SALT);

  return 0;
}
