#pragma once

#include "Logs.hpp"
#include <Database.hpp>

#include <sqlite3.h>

class Transaction {

public:
  explicit Transaction(Database &db) : db_(db) {
    exec("BEGIN TRANSACTION;").or_else([](auto &e) { log_error(e); });
  }

  Transaction(const Transaction &) = delete;
  Transaction &operator=(const Transaction &) = delete;

  Transaction(const Transaction &&) = delete;
  Transaction &operator=(const Transaction &&) = delete;

  ~Transaction();

  void commit();

private:
  Database &db_;
  Result<Unit, DbError> exec(const char *sql);
  bool commited = false;
};
