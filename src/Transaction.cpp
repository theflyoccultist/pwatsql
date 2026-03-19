#include <Logs.hpp>
#include <Transaction.hpp>

void Transaction::commit() {
  auto result = exec("COMMIT;");
  if (!result) {
    Logger::error(result.error());
    commited = false;
    return;
  }

  commited = true;
};

Transaction::~Transaction() {
  if (!commited) {
    auto result = exec("ROLLBACK;");

    if (!result) {
      Logger::error(result.error());
    }
  }
}

Result<Unit, DbError> Transaction::exec(const char *sql) {
  if (sqlite3_exec(db_.get(), sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
    return Result<Unit, DbError>::err(DbError::TransactionError);
  }
  return Result<Unit, DbError>::ok(Unit{});
}
