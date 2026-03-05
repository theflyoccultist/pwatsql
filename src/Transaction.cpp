#include "ErrorHandling.hpp"
#include <Transaction.hpp>

void Transaction::commit() {
  exec("COMMIT;");
  commited = true;
};

Transaction::~Transaction() {
  if (!commited) {
    exec("ROLLBACK;");
  }
}

Result<Unit, DbError> Transaction::exec(const char *sql) {
  if (sqlite3_exec(db_.get(), sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
    return Result<Unit, DbError>::err(DbError::TransactionError);
  }
  return Result<Unit, DbError>::ok(Unit{});
}
