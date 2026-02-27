#pragma once

#include <sqlite3.h>
#include <string>

class Database {
public:
  explicit Database(const std::string &filename);

  ~Database();

  sqlite3 *get() const noexcept;

private:
  sqlite3 *db_;
};
