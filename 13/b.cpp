
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

static const size_t PUZZLE_INPUT = 1350;
static const int TARGET_DISTANCE = 50;
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
  assert(distance <= TARGET_DISTANCE);
  g_visited[pos] = { pos, prev_pos, distance };
  const auto &moves = valid_moves(pos);
  for (const auto &p : moves) {
    g_to_visit.push({ p, pos, distance + 1 });
  }
}

bool out_of_options(const Node &node) {
  return node.distance > TARGET_DISTANCE;
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

  std::cout << "Visited nodes: " << g_visited.size() << std::endl;

  return 0;
}
