#pragma once

#include <sqlite3.h>

#include <DbError.hpp>
#include <ErrorHandling.hpp>

class Database {
public:
  static Result<Database, DbError> open(const char *path);

  ~Database();

  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;

  Database(Database &&other) noexcept : db_(other.db_) { other.db_ = nullptr; }

  Database &operator=(Database &&other) noexcept {
    if (this != &other) {
      if (db_ != nullptr) {
        sqlite3_close(db_);
      }

      db_ = other.db_;
      other.db_ = nullptr;
    }

    return *this;
  }

  [[nodiscard]] sqlite3 *get() const noexcept;

private:
  sqlite3 *db_ = nullptr;
  explicit Database(sqlite3 *db) : db_(db) {}
};
