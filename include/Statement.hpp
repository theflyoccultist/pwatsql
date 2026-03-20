#pragma once

#include <Asset.hpp>
#include <DbError.hpp>
#include <ErrorHandling.hpp>
#include <sqlite3.h>

class Statement {
public:
  static ResultT<Statement> prepare(sqlite3 *db, const char *query);
  void reset();
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

  void bind(int index, const Value &v);

  ResultT<bool> step();
  Value column(int index);

private:
  sqlite3_stmt *stmt_ = nullptr;
  explicit Statement(sqlite3_stmt *stmt) : stmt_(stmt) {}
};
