
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool starts_with(const std::string &str, const std::string &pattern) {
  return str.find(pattern) == 0;
}

void skip_words(std::istringstream &iss, int count) {
  std::string dummy;
  while (count--) {
    iss >> dummy;
  }
}

struct Sequence {
  void move_position(int source_index, int target_index) {
    const auto c = value[source_index];
    value.erase(value.begin() + source_index);
    value.insert(value.begin() + target_index, c);
  }

  void swap_letters(char c1, char c2) {
    swap_positions(value.find(c1), value.find(c2));
  }

  void swap_positions(int p1, int p2) {
    std::swap(value[p1], value[p2]);
  }

  void reverse_positions(int p1, int p2) {
    std::reverse(value.begin() + p1, value.begin() + p2 + 1);
  }

  void rotate_direction(bool is_left, int steps) {
    if (is_left) {
      std::rotate(value.begin(), value.begin() + steps, value.end());
    } else {
      std::rotate(value.rbegin(), value.rbegin() + steps, value.rend());
    }
  }

  void rotate_from_letter(char c) {
    const auto index = value.find(c);
    const auto rotations = 1 + index + (index >= 4 ? 1 : 0);
    rotate_direction(false, rotations % value.size());
  }

  std::string value;
};

int main() {
  std::ifstream ifs;

  //const char *input = "abcde";
  //ifs.open("input2");
  const char *input = "abcdefgh";
  ifs.open("input");

  Sequence seq = { input };

  std::string line;
  while (std::getline(ifs, line)) {
    std::istringstream iss(line);
    if (starts_with(line, "swap position")) {
      int p1, p2;
      skip_words(iss, 2);  // "swap position"
      iss >> p1;
      skip_words(iss, 2);  // "with position"
      iss >> p2;

      seq.swap_positions(p1, p2);
    } else if (starts_with(line, "swap letter")) {
      char c1, c2;
      skip_words(iss, 2);  // "swap letter"
      iss >> c1;
      skip_words(iss, 2);  // "with letter"
      iss >> c2;

      seq.swap_letters(c1, c2);
    } else if (starts_with(line, "rotate based")) {
      char c;
      skip_words(iss, 6);  // "rotate based on position of letter"
      iss >> c;

      seq.rotate_from_letter(c);
    } else if (starts_with(line, "rotate ")) {
      std::string dir;
      int steps;

      skip_words(iss, 1);  // "rotate"
      iss >> dir;
      iss >> steps;
      skip_words(iss, 1);  // "step"

      assert(dir == "left" || dir == "right");
      seq.rotate_direction(dir == "left", steps);
    } else if (starts_with(line, "reverse ")) {
      int p1, p2;
      skip_words(iss, 2);  // "reverse positions"
      iss >> p1;
      skip_words(iss, 1);  // "through"
      iss >> p2;

      seq.reverse_positions(p1, p2);
    } else if (starts_with(line, "move ")) {
      int p1, p2;
      skip_words(iss, 2);  // "move position"
      iss >> p1;
      skip_words(iss, 2);  // "to position"
      iss >> p2;

      seq.move_position(p1, p2);
    }

    std::cout << " " << seq.value << std::endl;
  }

  ifs.close();

  return 0;
}
