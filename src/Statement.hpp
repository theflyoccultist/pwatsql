#pragma once

#include <DbError.hpp>
#include <ErrorHandling.hpp>
#include <cstdint>
#include <sqlite3.h>
#include <string>

class Statement {
public:
  static ResultT<Statement> prepare(sqlite3 *db, const char *query);

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

  ResultT<Unit> bind_int(int index, int value);
  ResultT<Unit> bind_text(int index, const std::string &value);
  ResultT<Unit> bind_int64(int index, std::int64_t value);

  ResultT<bool> step();

  int column_int(int index);
  std::int64_t column_int64(int index);
  std::string column_text(int index);

private:
  sqlite3_stmt *stmt_ = nullptr;
  explicit Statement(sqlite3_stmt *stmt) : stmt_(stmt) {}
};
