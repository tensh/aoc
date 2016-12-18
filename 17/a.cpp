
#include <algorithm>
#include <array>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include "md5.h"

enum Cell {
  EMPT,
  WALL,
  DOOR,
  VAUL
};

std::string OPEN_DOOR_CHARS("bcdef");

//  #########
//  #S| | | #
//  #-#-#-#-#
//  # | | | #
//  #-#-#-#-#
//  # | | | #
//  #-#-#-#-#
//  # | | |V
//  #######

using Row = std::array<Cell, 9>;
using Grid = std::array<Row, 9>;

Grid g_grid = {
  Row { WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL },
  Row { WALL, EMPT, DOOR, EMPT, DOOR, EMPT, DOOR, EMPT, WALL },
  Row { WALL, DOOR, WALL, DOOR, WALL, DOOR, WALL, DOOR, WALL },
  Row { WALL, EMPT, DOOR, EMPT, DOOR, EMPT, DOOR, EMPT, WALL },
  Row { WALL, DOOR, WALL, DOOR, WALL, DOOR, WALL, DOOR, WALL },
  Row { WALL, EMPT, DOOR, EMPT, DOOR, EMPT, DOOR, EMPT, WALL },
  Row { WALL, DOOR, WALL, DOOR, WALL, DOOR, WALL, DOOR, WALL },
  Row { WALL, EMPT, DOOR, EMPT, DOOR, EMPT, DOOR, VAUL, EMPT },
  Row { WALL, WALL, WALL, WALL, WALL, WALL, WALL, EMPT, EMPT }
};

struct Point {
  Point operator+(const Point &rhs) const {
    return { x + rhs.x, y + rhs.y };
  }

  int x;
  int y;
};

struct Node {
  bool operator>(const Node &rhs) const {
    return distance > rhs.distance;
  }

  size_t distance;
  Point pos;
  std::string moves;
};

std::array<bool, 4> find_unlockables(const std::string &input) {
  const auto &hashed = md5(input);

  std::array<bool, 4> moves;
  std::transform(hashed.begin(), hashed.begin() + 4, moves.begin(), [] (char c) {
    return OPEN_DOOR_CHARS.find(c) != std::string::npos;
  });

  return moves;
}

bool possible_move(const Point &pos, bool can_unlock) {
  const auto cell = g_grid[pos.y][pos.x];

  return
      cell != WALL &&
      (cell != DOOR || can_unlock);
}

std::array<bool, 4> possible_moves(const Point &pos, const std::string &input) {
  const auto &unlockables = find_unlockables(input);

  const Point up    = { pos.x, pos.y - 1 };
  const Point down  = { pos.x, pos.y + 1 };
  const Point left  = { pos.x - 1, pos.y };
  const Point right = { pos.x + 1, pos.y };

  return {
      possible_move(up, unlockables[0]),
      possible_move(down, unlockables[1]),
      possible_move(left, unlockables[2]),
      possible_move(right, unlockables[3]) };
}

std::string solve(const std::string &puzzle) {
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pqueue;

  pqueue.push({ 0, Point { 1, 1 }, "" });

  while (!pqueue.empty()) {
    const auto node = pqueue.top();
    pqueue.pop();

    if (g_grid[node.pos.y][node.pos.x] == VAUL) {
      return node.moves;
    }

    const auto d = node.distance;
    const auto moves = possible_moves(node.pos, puzzle + node.moves);

    if (moves[0]) pqueue.push({ d, node.pos + Point { 0, -2 }, node.moves + 'U' });
    if (moves[1]) pqueue.push({ d, node.pos + Point { 0,  2 }, node.moves + 'D' });
    if (moves[2]) pqueue.push({ d, node.pos + Point { -2, 0 }, node.moves + 'L' });
    if (moves[3]) pqueue.push({ d, node.pos + Point {  2, 0 }, node.moves + 'R' });
  }

  return "No solution found.";
}

void print_grid(const Grid &grid) {
  std::for_each(grid.begin(), grid.end(), [] (const Row &row) {
    std::for_each(row.begin(), row.end(), [] (const Cell &cell) {
      char c = 'X';
      switch (cell) {
        case EMPT: c = ' '; break;
        case WALL: c = '#'; break;
        case DOOR: c = '/'; break;
        case VAUL: c = 'V'; break;
      }
      std::cout << c;
    });
    std::cout << std::endl;
  });
}

int main() {
  const char *puzzle = "edjrjqaa";
  //const char *puzzle = "hijkl";

  std::cout << solve(puzzle) << std::endl;

  return 0;
}
