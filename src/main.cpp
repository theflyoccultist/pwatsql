#include "AssetRepository.hpp"
#include "Database.hpp"

int main() {
  Database db("db.sqlite3");
  AssetRepository repo(db);
  repo.createTable();
  repo.insertData({1, "music", "music/loudboom.wav", 1000000, "combat,menu"});
  repo.insertData({2, "sfx", "sfx/danger.ogg", 1654654, "enemy,action"});
  repo.updateData({2, "sfx", "sfx/angelic.ogg", 188888, "ally,luck"});
  repo.deleteSelectedRow(2);

  auto asset = repo.getSelectedRow(1);
  auto assets = repo.getAllRows();
}
