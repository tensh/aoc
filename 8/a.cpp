
#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <string>

const int COLUMNS = 50;
const int ROWS = 6;

using Row = std::array<bool, COLUMNS>;

class Display {
 public:
  Display() {
    std::for_each(_matrix.begin(), _matrix.end(), [] (Row &row) {
      row.fill(false);
    });
  }

  void rect(int rows, int cols) {
    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        _matrix[r][c] = true;
      }
    }
  }

  void rotate_column(int column, int steps) {
    std::vector<bool> col;
    std::transform(_matrix.begin(), _matrix.end(), std::back_inserter(col), [column] (const Row &row) {
      return row[column];
    });

    std::array<Row, ROWS> new_matrix(_matrix);
    for (int r = 0; r < ROWS; ++r) {
      Row &new_row = new_matrix[r];
      int i = r - steps;
      while (i < 0) {
        i += ROWS;
      }
      new_row[column] = _matrix[i][column];
    }

    _matrix.swap(new_matrix);
  }

  void rotate_row(int row, int steps) {
    Row new_row;
    for (int c = 0; c < COLUMNS; ++c) {
      const int i = (c + steps) % COLUMNS;
      new_row[i] = _matrix[row][c];
    }
    _matrix[row].swap(new_row);
  }

  void show() const {
    std::cout << "DISPLAY:" << std::endl;
    std::for_each(_matrix.begin(), _matrix.end(), &show_row);
    std::cout << std::endl;
  }

  int lit_lights() const {
    std::vector<int> row_values;
    std::transform(_matrix.begin(), _matrix.end(), std::back_inserter(row_values), [] (const Row &row) {
      return std::count(row.begin(), row.end(), true);
    });

    return std::accumulate(row_values.begin(), row_values.end(), 0, [] (int sum, int v) {
      return sum + v;
    });
  }

 private:
  static void show_row(const Row &row) {
    std::transform(row.begin(), row.end(), std::ostream_iterator<char>(std::cout, ""), [] (bool lit) {
      return lit ? '#' : '.';
    });
    std::cout << std::endl;
  }

  std::array<Row, ROWS> _matrix;
};

bool starts_with(const std::string &input, const std::string &test) {
  return input.find(test) == 0;
}

void read_rect(const std::string &input, int &rows, int &cols) {
  std::string rest = input.substr(std::string("rect ").size());
  const auto x_index = rest.find('x');
  std::string scols = rest.substr(0, x_index);
  std::string srows = rest.substr(x_index + 1);

  std::cout << "Rows: " << srows << ", Cols: " << scols << std::endl;
  rows = stoi(srows);
  cols = stoi(scols);
}

void read_rotate(const std::string &input, int &index, int &steps) {
  const auto far = std::numeric_limits<std::streamsize>::max();

  std::istringstream iss(input);
  iss.ignore(far, '=');  // "rotate column/row x/y"
  iss >> index;
  iss.ignore(far, ' ');  // " "
  iss.ignore(far, ' ');  // "by"
  iss >> steps;
}

void run_command(Display &display, const std::string &line) {
  if (starts_with(line, "rect ")) {
    int rows, cols;
    read_rect(line, rows, cols);
    display.rect(rows, cols);
  } else if (starts_with(line, "rotate column ")) {
    int col, steps;
    read_rotate(line, col, steps);
    display.rotate_column(col, steps);
  } else if (starts_with(line, "rotate row ")) {
    int row, steps;
    read_rotate(line, row, steps);
    display.rotate_row(row, steps);
  } else {
    throw "Unknown command!";
  }
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  Display display;
  display.show();

  std::string line;
  while (std::getline(ifs, line)) {
    std::cout << "> " << line << std::endl;
    run_command(display, line);
    display.show();
  }

  std::cout << "Lit lights: " << display.lit_lights() << std::endl;

  ifs.close();

  return 0;
}
