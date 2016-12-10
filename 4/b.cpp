
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
    const size_t sector_index = line.find_last_of('-'); 
    const auto &name = line.substr(0, sector_index);
    const size_t del_index = line.find('[');
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

  std::string checksum;
  std::transform(chars.begin(), chars.begin() + 5, std::back_inserter(checksum), [] (const Pair &p) {
    return p.first;
  });

  return checksum;
}

bool valid_room_name(const RoomInfo &room_info) {
  const auto &real_checksum = calculate_checksum(room_info.name);
  return room_info.checksum == real_checksum;
}

std::string decrypt_room_name(const RoomInfo &room_info) {
  const int delta = room_info.sector_id % 26;
  const std::string &name = room_info.name;

  std::string decrypted;
  std::transform(name.begin(), name.end(), std::back_inserter(decrypted), [delta] (const char c) {
    if (isalpha(c)) {
      int new_c = c + delta;
      return new_c > 122 ? (char)(new_c - 26) : (char)new_c;
    }
    return c;
  });

  return decrypted;
}

int main() {
  std::ifstream ifs;
  ifs.open("input");

  std::string line;
  while (std::getline(ifs, line)) {
    const auto &room_info = RoomInfo::parse(line);
    if (valid_room_name(room_info)) {
      std::cout << decrypt_room_name(room_info) << " (" << room_info.sector_id << ")" << std::endl;
    }
  }

  ifs.close();

  return 0;
}
