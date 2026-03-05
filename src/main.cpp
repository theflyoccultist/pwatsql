#include <AssetRepository.hpp>
#include <Database.hpp>
#include <ErrorHandling.hpp>
#include <Logs.hpp>
#include <Transaction.hpp>
#include <chrono>
#include <ctime>

int main() {
  const auto now = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

  auto db_result = Database::open("db.sqlite3");
  if (!db_result) {
    log_error(db_result.error());
    return 1;
  }

  Database db = std::move(db_result.value());
  AssetRepository repo(db);

  Transaction txn(db);

  repo.createTable();

  repo.insertData({.id = 1,
                   .type = "music",
                   .path = "music/loudboom.wav",
                   .last_modified = t_c,
                   .tags = "combat,menu"});

  repo.insertData({.id = 2,
                   .type = "sfx",
                   .path = "sfx/danger.ogg",
                   .last_modified = t_c,
                   .tags = "enemy,action"});

  repo.updateData({.id = 2,
                   .type = "sfx",
                   .path = "sfx/angelic.ogg",
                   .last_modified = t_c,
                   .tags = "ally,luck"});

  repo.deleteSelectedRow(2);

  txn.commit();

  // auto asset = repo.getSelectedRow(1);
  // auto assets = repo.getAllRows();

  return 0;
}
