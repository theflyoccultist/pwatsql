#pragma once

#include <cstdint>
#include <ostream>
#include <string>

struct NewAsset {
  std::string type;
  std::string path;
  std::int64_t last_modified;
  std::string tags;
};

struct AssetUpdate {
  int id;
  std::string type;
  std::string path;
  std::int64_t last_modified;
  std::string tags;
};

struct Asset {
  int id;
  std::string type;
  std::string path;
  std::int64_t last_modified;
  std::string tags;
};

inline std::ostream &operator<<(std::ostream &os, const Asset &a) {
  os << "Asset { "
     << "id : " << a.id << ", type: " << a.type << ", path: " << a.path
     << ", last_modified: " << a.last_modified << ", tags: " << a.tags << " }";
  return os;
}
