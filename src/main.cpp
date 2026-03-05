#include <AssetRepository.hpp>
#include <Database.hpp>
#include <ErrorHandling.hpp>
#include <iostream>

int main() {
  auto db_result = Database::open("db.sqlite3");
  if (!db_result) {
    std::cerr << "DB Open Failed";
    return 1;
  }

  Database db = std::move(db_result.value());
  AssetRepository repo(db);
  repo.createTable();
  repo.insertData({1, "music", "music/loudboom.wav", 1000000, "combat,menu"});
  repo.insertData({2, "sfx", "sfx/danger.ogg", 1654654, "enemy,action"});
  repo.updateData({2, "sfx", "sfx/angelic.ogg", 188888, "ally,luck"});
  repo.deleteSelectedRow(2);

  auto asset = repo.getSelectedRow(1);
  auto assets = repo.getAllRows();

  return 0;
}
