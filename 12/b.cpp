
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// cpy <int> <id>
// jnz <int|id> <int>
// inc <id>
// dev <id>

using Registers = std::map<std::string, int>;

Registers g_registers;

void set_value(const std::string &id, int value) {
  g_registers[id] = value;
}

int &get_value(const std::string &id) {
  if (g_registers.count(id) == 0) {
    set_value(id, 0);
  }
  return g_registers[id];
}

void print_registers() {
  std::cout << "Register contents:" << std::endl;
  for (const auto &p : g_registers) {
    std::cout << "  [" << p.first << "]:\t" << p.second << std::endl;
  }
}

std::string parse_id(std::istringstream &iss) {
  std::string id;
  if (!(iss >> id)) throw "Invalid input.";
  return id;
}

bool try_parse_int(std::istringstream &iss, int &output, std::string &text) {
  const auto str(parse_id(iss));
  try {
    output = std::stoi(str);
    return true;
  } catch (std::invalid_argument &ex) {
    text = str;
    return false;
  }
}

int parse_int(std::istringstream &iss) {
  int x;
  std::string dummy;
  if (!try_parse_int(iss, x, dummy)) {
    std::cerr << "Failed to parse '" << dummy << "' as integer." << std::endl;
    throw "Invalid input. Expected integer.";
  }
  return x;
}

int parse_value(std::istringstream &iss) {
  int branch_int;
  std::string branch_id;
  return try_parse_int(iss, branch_int, branch_id) ? branch_int : get_value(branch_id);
}

void execute(const std::vector<std::string> &lines) {
  for (int i = 0; i < lines.size(); ++i) {
    const auto &line = lines[i];

    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    if (cmd == "cpy") {
      // cpy <int> <id>
      const auto v = parse_value(iss);
      const auto &id = parse_id(iss);
      set_value(id, v);
      //std::cout << i << "\tCopy " << v << " to " << id << std::endl;
    } else if (cmd == "jnz") {
      // jnz <int|id> <int>
      const int cond = parse_value(iss);
      const int distance = parse_int(iss);
      if (cond != 0) {
        i += distance - 1;
        //std::cout << i << "\tJump " << distance << " steps." << std::endl;
      }
    } else if (cmd == "inc") {
      // inc <id>
      const auto &id = parse_id(iss);
      ++get_value(id);
      //std::cout << i << "\tIncrement " << id << std::endl;
    } else if (cmd == "dec") {
      // dev <id>
      const auto &id = parse_id(iss);
      --get_value(id);
      //std::cout << i << "\tDecrement " << id << std::endl;
    }
  }
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  g_registers["c"] = 1;

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(ifs, line)) {
    lines.push_back(line);
  }

  execute(lines);
  print_registers();

  ifs.close();

  return 0;
}
