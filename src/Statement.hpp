#pragma once

#include <cstdint>
#include <sqlite3.h>
#include <stdexcept>
#include <string>
#include <string_view>

class Statement {
public:
  Statement(sqlite3 *db, std::string_view sql) : db_(db), stmt_(nullptr) {
    int rc = sqlite3_prepare_v2(db_, sql.data(), -1, &stmt_, nullptr);
    if (rc != SQLITE_OK)
      throw std::runtime_error(sqlite3_errmsg(db_));
  };

  ~Statement() {
    if (stmt_)
      sqlite3_finalize(stmt_);
  };

  sqlite3_stmt *get() const noexcept { return stmt_; }

  void bind(int index, int value) {
    if (sqlite3_bind_int(stmt_, index, value) != SQLITE_OK)
      throw std::runtime_error(sqlite3_errmsg(db_));
  };

  void bind(int index, const std::string &value) {
    if (sqlite3_bind_text(stmt_, index, value.c_str(), -1, nullptr) !=
        SQLITE_OK)
      throw std::runtime_error(sqlite3_errmsg(db_));
  };

  bool step() {
    int rc = sqlite3_step(stmt_);
    if (rc == SQLITE_ROW)
      return true;
    if (rc == SQLITE_DONE)
      return false;
    throw std::runtime_error(sqlite3_errmsg(db_));
  }

  int column_int(int index) { return sqlite3_column_int(stmt_, index); }

  std::string column_text(int index) {
    const unsigned char *text = sqlite3_column_text(stmt_, index);
    return text ? reinterpret_cast<const char *>(text) : "";
  };

  std::int64_t column_int64(int index) {
    return sqlite3_column_int64(stmt_, index);
  }

private:
  sqlite3 *db_;
  sqlite3_stmt *stmt_;
};
