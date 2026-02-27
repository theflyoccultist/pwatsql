#include "Database.hpp"
#include <stdio.h>

Database::Database(const std::string &filename) {
  int opened = sqlite3_open(filename.c_str(), &db_);
  if (opened) {
    printf("Database could not be opened %s \n", sqlite3_errmsg(db_));
  } else {
    printf("Opened database successfully \n");
  }
};

Database::~Database() {
  if (db_) {
    sqlite3_close(db_);
    printf("sqlite3 has been closed\n");
  }
}

sqlite3 *Database::get() const noexcept { return db_; };
