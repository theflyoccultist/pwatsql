#include <AssetRepository.hpp>
#include <Database.hpp>
#include <ErrorHandling.hpp>
#include <Logs.hpp>
#include <Transaction.hpp>
#include <chrono>
#include <ctime>
#include <string>

int main(int argc, char **argv) {
  bool verbose = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[1];

    if (arg == "-v" || arg == "--verbose") {
      verbose = true;
    }
  }

  verbose ? logger::set_level(LogLevel::Info)
          : logger::set_level(LogLevel::Error);

  // Test Program

  const auto now = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

  auto db_result = Database::open("db.sqlite3");
  if (!db_result) {
    logger::error(db_result.error());
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

  // repo.deleteSelectedRow(2);

  txn.commit();

  auto asset = repo.getAssetById(1);
  asset.or_else(logger::error);

  if (asset) {
    logger::info(asset.value());
  }

  auto assets = repo.getAllAssets();

  for (const auto &a : assets.value()) {
    logger::info(a);
  }

  auto tags = repo.getAssetsByTag("ally");

  for (const auto &t : tags.value()) {
    logger::info(t);
  }

  auto types = repo.getAssetsByType("sfx");

  for (const auto &t : types.value()) {
    logger::info(t);
  }

  return 0;
}
