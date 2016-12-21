
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct Range {
  bool overlaps(const Range &rhs) const {
    return
        point_within(rhs.begin) ||
        point_within(rhs.end) ||
        contains(rhs) ||
        rhs.contains(*this);
  }

  bool contains(const Range &rhs) const {
    return rhs.begin >= begin && rhs.end <= end;
  }

  bool point_within(unsigned int v) const {
    return v >= begin && v <= end;
  }

  unsigned int begin;
  unsigned int end;
};

bool begin_lt(const Range &lhs, const Range &rhs) {
  return lhs.begin < rhs.begin;
}

struct Ranges {
  void absorb(const Range &range) {
    Range r(range);

    for (auto it = ranges.begin(); it != ranges.end();) {
      if (range.overlaps(*it)) {
        r.begin = std::min(r.begin, it->begin);
        r.end = std::max(r.end, it->end);
        it = ranges.erase(it);
      } else {
        ++it;
      }
    }

    insert_sorted(r);
  }

  void insert_sorted(const Range &range) {
    ranges.insert(
        std::upper_bound(ranges.begin(), ranges.end(), range, &begin_lt),
        range);
  }

  unsigned int valid_ips() const {
    assert(ranges.size() > 0);
    unsigned int valid = 0;

    if (ranges[0].begin > 0) {
      valid += ranges[0].begin;
    }

    for (int i = 0; i < ranges.size() - 1; ++i) {
      valid += ranges[i+1].begin - ranges[i].end - 1;
    }
    return valid;
  }

  void print() const {
    for (const auto &range : ranges) {
      std::cout << "[" << range.begin << ", " << range.end << "]" << std::endl;
    }
  }

  std::vector<Range> ranges;
};

int main() {
  std::ifstream ifs;
  ifs.open("input");

  Ranges ranges;
  std::string line;
  while (std::getline(ifs, line)) {
    std::istringstream iss(line);

    unsigned int begin, end;
    iss >> begin;
    iss.ignore(1);
    iss >> end;

    ranges.absorb({ begin, end });
  }

  ranges.print();

  std::cout << "Valid IP addresses: " << ranges.valid_ips() << std::endl;

  ifs.close();

  return 0;
}
