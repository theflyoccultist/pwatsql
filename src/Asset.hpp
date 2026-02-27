#pragma once

#include <cstdint>
#include <string>

struct Asset {
  int id;
  std::string type;
  std::string path;
  std::int64_t last_modified;
  std::string tags;
};
