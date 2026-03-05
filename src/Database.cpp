#include <Database.hpp>
#include <ErrorHandling.hpp>
#include <cstdio>
#include <iostream>

Result<Database, DbError> Database::open(const char *path) {
  sqlite3 *db = nullptr;

  if (sqlite3_open(path, &db) != SQLITE_OK) {
    return Result<Database, DbError>::err(DbError::OpenFailed);
  }

  std::cout << "sqlite3 has been opened\n";
  return Result<Database, DbError>::ok(Database(db));
}

Database::~Database() {
  if (db_ != nullptr) {
    sqlite3_close_v2(db_);
    std::cout << "sqlite3 has been closed\n";
  }
}

sqlite3 *Database::get() const noexcept { return db_; };
