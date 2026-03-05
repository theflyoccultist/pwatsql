#include <ErrorHandling.hpp>
#include <Statement.hpp>

Result<Statement, DbError> Statement::prepare(sqlite3 *db, const char *query) {
  sqlite3_stmt *stmt = nullptr;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
    return Result<Statement, DbError>::err(DbError::PrepareFailed);
  };
  return Result<Statement, DbError>::ok(Statement(stmt));
}

Statement::~Statement() {
  if (stmt_ != nullptr) {
    sqlite3_finalize(stmt_);
  }
};

Result<Unit, DbError> Statement::bind_int(int index, int value) {
  if (sqlite3_bind_int(stmt_, index, value) != SQLITE_OK) {
    return Result<Unit, DbError>::err(DbError::BindFailed);
  }
  return Result<Unit, DbError>::ok(Unit{});
};

Result<Unit, DbError> Statement::bind_int64(int index, std::int64_t value) {
  if (sqlite3_bind_int64(stmt_, index, value) != SQLITE_OK) {
    return Result<Unit, DbError>::err(DbError::BindFailed);
  }
  return Result<Unit, DbError>::ok(Unit{});
};

Result<Unit, DbError> Statement::bind_text(int index,
                                           const std::string &value) {
  if (sqlite3_bind_text(stmt_, index, value.c_str(), -1, nullptr) !=
      SQLITE_OK) {
    return Result<Unit, DbError>::err(DbError::BindFailed);
  }
  return Result<Unit, DbError>::ok(Unit{});
};

Result<bool, DbError> Statement::step() {
  int rc = sqlite3_step(stmt_);
  if (rc == SQLITE_ROW) {
    return Result<bool, DbError>::ok(true);
  }

  if (rc == SQLITE_DONE) {
    return Result<bool, DbError>::ok(false);
  }

  return Result<bool, DbError>::err(DbError::ConstraintError);
}

int Statement::column_int(int index) {
  return sqlite3_column_int(stmt_, index);
}

std::int64_t Statement::column_int64(int index) {
  return sqlite3_column_int64(stmt_, index);
}

std::string Statement::column_text(int index) {
  const unsigned char *text = sqlite3_column_text(stmt_, index);
  return (text != nullptr) ? reinterpret_cast<const char *>(text) : "";
}
