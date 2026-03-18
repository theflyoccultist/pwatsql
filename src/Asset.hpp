#pragma once

#include <cstdint>
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
