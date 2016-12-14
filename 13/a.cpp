
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>

struct Point {
  const bool operator<(const Point &rhs) const {
    return x < rhs.x || (x == rhs.x && y < rhs.y);
  }

  const bool operator==(const Point &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  int x;
  int y;
};

//static const size_t PUZZLE_INPUT = 10;
//static const Point GOAL = { 7, 4 };
static const size_t PUZZLE_INPUT = 1350;
static const Point GOAL = { 31, 39 };
static const Point START = { 1, 1 };

struct Node {
  const bool operator>(const Node &rhs) const {
    return distance > rhs.distance;
  }

  Point pos;
  Point prev_pos;
  int distance;
};

using Visited = std::map<Point, Node>;
Visited g_visited;

using ToVisit = std::priority_queue<Node, std::vector<Node>, std::greater<Node>>;
ToVisit g_to_visit;

bool is_wall(const Point &p) {
  const int x = p.x;
  const int y = p.y;

  if (x < 0 || y < 0) {
    return true;
  }

  size_t v = x*x + 3*x + 2*x*y + y + y*y + PUZZLE_INPUT;
  int bits_set = 0;
  while (v > 0) {
    if (v & 0x1) ++bits_set;
    v = v >> 1;
  }
  return (bits_set % 2) == 1;
}

std::vector<Point> valid_moves(const Point &p) {
  const std::array<Point, 4> moves = {
      Point { p.x - 1, p.y },
      Point { p.x + 1, p.y },
      Point { p.x, p.y - 1 },
      Point { p.x, p.y + 1 } };
  std::vector<Point> valid;
  std::copy_if(moves.begin(), moves.end(), std::back_inserter(valid), [] (const Point &p) {
    const auto it = g_visited.find(p);
    if (it != g_visited.end()) {
      // We're only interested if it's closer than the previous path we found.
      return it->second.distance > (g_visited[p].distance + 1);
    }
    return !is_wall(p);
  });
  return valid;
}

void visit(const Point &pos, const Point &prev_pos) {
  const auto distance = g_visited[prev_pos].distance + 1;
  g_visited[pos] = { pos, prev_pos, distance };
  const auto &moves = valid_moves(pos);
  for (const auto &p : moves) {
    g_to_visit.push({ p, pos, distance + 1 });
  }
}

void find_solution_measurements(const std::vector<Point> &solution, int &max_x, int &max_y) {
  max_x = 0;
  max_y = 0;

  for (const auto &p : solution) {
    max_x = std::max(max_x, p.x);
    max_y = std::max(max_y, p.y);
  }
  ++max_x;
  ++max_y;
}

void print_map(const std::vector<Point> &solution, bool show_solution) {
  int width, height;
  find_solution_measurements(solution, width, height);

  std::cout << "\t";
  for (int x = 0; x < width; ++x) {
    std::cout << " " << (x % 10);
  }
  std::cout << std::endl << std::endl;

  for (int y = 0; y < height; ++y) {
    std::cout << y << "\t";
    for (int x = 0; x < width; ++x) {
      const Point p = { x, y };
      const bool in_solution = std::find(solution.begin(), solution.end(), p) != solution.end();
      std::cout << " " <<
          ( (in_solution && show_solution) ? 'O' :
            is_wall(p) ? '#' :
            '.');
    }
    std::cout << std::endl;
  }
}

bool out_of_options(const Node &node) {
  const auto it = g_visited.find(GOAL);
  return it != g_visited.end() && it->second.distance <= (node.distance + 1);
}

std::vector<Point> get_solution() {
  std::vector<Point> result;

  auto *node = &g_visited[GOAL];
  while (!(node->pos == START)) {
    result.insert(result.begin(), node->pos);
    node = &g_visited[node->prev_pos];
  }
  result.insert(result.begin(), START);
  return result;
}

int main() {
  // Initialize map of visited nodes.
  Node start_node = { START, START, -1 };
  g_visited[START] = start_node;
  g_to_visit.push(start_node);

  while (!out_of_options(g_to_visit.top())) {
    const auto node = g_to_visit.top();
    g_to_visit.pop();
    visit(node.pos, node.prev_pos);
  }

  const auto &solution = get_solution();
  print_map(solution, false);
  std::cout << std::endl;
  print_map(solution, true);

  std::cout << std::endl << "Goal distance: " << g_visited[GOAL].distance << std::endl;
  std::cout << "Positions in solution: " << solution.size() << std::endl;

  return 0;
}
