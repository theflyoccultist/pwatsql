#pragma once

#include <ErrorHandling.hpp>
#include <cstdint>
#include <sqlite3.h>
#include <string>

class Statement {
public:
  static Result<Statement, DbError> prepare(sqlite3 *db, const char *query);

  ~Statement();

  Statement(const Statement &) = delete;
  Statement &operator=(const Statement &) = delete;

  Statement(Statement &&other) noexcept : stmt_(other.stmt_) {
    other.stmt_ = nullptr;
  }

  Statement &operator=(Statement &&other) noexcept {
    if (this != &other) {
      if (stmt_ != nullptr) {
        sqlite3_finalize(stmt_);
      }

      stmt_ = other.stmt_;
      other.stmt_ = nullptr;
    }
    return *this;
  }

  [[nodiscard]] sqlite3_stmt *get() const noexcept { return stmt_; }

  Result<Unit, DbError> bind_int(int index, int value);
  Result<Unit, DbError> bind_text(int index, const std::string &value);
  Result<Unit, DbError> bind_int64(int index, std::int64_t value);

  Result<bool, DbError> step();

  int column_int(int index);
  std::int64_t column_int64(int index);
  std::string column_text(int index);

private:
  sqlite3_stmt *stmt_ = nullptr;
  explicit Statement(sqlite3_stmt *stmt) : stmt_(stmt) {}
};
