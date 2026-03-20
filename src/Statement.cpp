#include <ErrorHandling.hpp>
#include <Statement.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

ResultT<Statement> Statement::prepare(sqlite3 *db, const char *query) {
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
    return ResultT<Statement>::err(DbError::PrepareFailed);
  };
  return ResultT<Statement>::ok(Statement(stmt));
}

void Statement::reset() {
  sqlite3_reset(stmt_);
  sqlite3_clear_bindings(stmt_);
}

Statement::~Statement() {
  if (stmt_ != nullptr) {
    sqlite3_finalize(stmt_);
  }
};

void Statement::bind(int index, const Value &v) {
  std::visit(
      [&](auto &&val) {
        using T = std::decay_t<decltype(val)>;

        if constexpr (std::is_same_v<T, int64_t>) {
          sqlite3_bind_int64(stmt_, index, val);
        } else if constexpr (std::is_same_v<T, double>) {
          sqlite3_bind_double(stmt_, index, val);
        } else if constexpr (std::is_same_v<T, std::string>) {
          sqlite3_bind_text(stmt_, index, val.c_str(), -1, nullptr);
        } else if constexpr (std::is_same_v<T, std::nullptr_t>) {
          sqlite3_bind_null(stmt_, index);
        }
      },
      v);
};

ResultT<bool> Statement::step() {
  int rc = sqlite3_step(stmt_);
  if (rc == SQLITE_ROW) {
    return ResultT<bool>::ok(true);
  }

  if (rc == SQLITE_DONE) {
    return ResultT<bool>::ok(false);
  }

  return ResultT<bool>::err(DbError::ConstraintError);
}

Value Statement::column(int index) {
  int type = sqlite3_column_type(stmt_, index);

  switch (type) {
  case SQLITE_INTEGER: {
    return sqlite3_column_int64(stmt_, index);
  }

  case SQLITE_FLOAT: {
    return sqlite3_column_double(stmt_, index);
  }

  case SQLITE_TEXT: {
    const unsigned char *text = sqlite3_column_text(stmt_, index);
    return (text != nullptr) ? reinterpret_cast<const char *>(text) : "";
  }

  case SQLITE_NULL: {
    return nullptr;
  }

  default: {
    throw std::runtime_error("Unsupported SQLite column type");
  }
  }
}
