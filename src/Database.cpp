#include <Database.hpp>
#include <ErrorHandling.hpp>
#include <stdio.h>

Result<Database, DbError> Database::open(const char *path) {
  sqlite3 *db = nullptr;

  if (sqlite3_open(path, &db) != SQLITE_OK) {
    return Result<Database, DbError>::err(DbError::OpenFailed);
  }

  return Result<Database, DbError>::ok(Database(db));
}

Database::~Database() {
  if (db_) {
    sqlite3_close_v2(db_);
    printf("sqlite3 has been closed\n");
  }
}

sqlite3 *Database::get() const noexcept { return db_; };
