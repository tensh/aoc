
#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

struct RoomInfo {
  static RoomInfo parse(const std::string &line) {
    const size_t del_index = line.find('[');
    const auto &name = line.substr(0, del_index);
    const size_t sector_index = line.find_last_of('-'); 
    const auto &sector_id = line.substr(sector_index + 1, del_index - sector_index - 1);
    const auto &checksum = line.substr(del_index + 1, line.find(']') - del_index - 1);

    return { name, stoi(sector_id), checksum };
  }

  const std::string name;
  const int sector_id;
  const std::string checksum;
};

std::string calculate_checksum(const std::string &name) {
  using CharMap = std::map<char, int>;
  using Pair = std::pair<char, int>;
  using CharVec = std::vector<Pair>;

  CharMap char_map;
  for (auto c : name) {
    if (isalpha(c)) {
      if (char_map.count(c) == 0) {
        char_map[c] = 1;
      } else {
        ++char_map[c];
      }
    }
  }

  CharVec chars;
  std::for_each(char_map.begin(), char_map.end(), [&chars] (const Pair &p) {
    chars.push_back(p);
  });

  std::sort(chars.begin(), chars.end(), [] (const Pair &p1, const Pair &p2) {
    if (p1.second == p2.second) {
      return p1.first < p2.first;  // ascending by name
    }
    return p2.second < p1.second;  // descending by value
  });

  for (const auto &p : chars) {
    std::cout << "(" << p.first << ", " << p.second << "), ";
  }
  std::cout << std::endl;

  std::string checksum;
  std::transform(chars.begin(), chars.begin() + 5, std::back_inserter(checksum), [] (const Pair &p) {
    return p.first;
  });

  return checksum;
}

int valid_sector_id(const std::string &line) {
  const auto room_info = RoomInfo::parse(line);
  const auto &real_checksum = calculate_checksum(room_info.name);

  std::cout << "Name part: " << room_info.name << std::endl;
  std::cout << "Sector ID: " << room_info.sector_id << std::endl;
  std::cout << "Checksum: " << room_info.checksum << std::endl;
  std::cout << "Real Checksum: " << real_checksum << std::endl;
  std::cout << "===" << std::endl;

  return room_info.checksum == real_checksum ? room_info.sector_id : 0;
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  int sum = 0;
  std::string line;
  while (std::getline(ifs, line)) {
    sum += valid_sector_id(line);
  }

  std::cout << "Sector ID sum: " << sum << std::endl;

  ifs.close();

  return 0;
}
