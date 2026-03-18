#include "AssetRegistry.hpp"
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
  AssetRegistry registry(repo);

  Transaction txn(db);

  repo.createTable().or_else(logger::error);

  repo.insertData({.type = "music",
                   .path = "music/loudboom.wav",
                   .last_modified = t_c,
                   .tags = "combat,menu"})
      .or_else(logger::error);

  repo.insertData({.type = "sfx",
                   .path = "sfx/danger.ogg",
                   .last_modified = t_c,
                   .tags = "enemy,action"})
      .or_else(logger::error);

  repo.insertData({.type = "texture",
                   .path = "texture/hero.png",
                   .last_modified = t_c,
                   .tags = "character,mesh"})
      .or_else(logger::error);

  repo.updateData({.id = 2,
                   .type = "sfx",
                   .path = "sfx/angelic.ogg",
                   .last_modified = t_c,
                   .tags = "ally,luck"})
      .or_else(logger::error);

  txn.commit();

  auto music = registry.getMusic();
  for (const auto &m : music.value()) {
    logger::info(m.path);
  }

  auto sfx = registry.getSfx();
  for (const auto &s : sfx.value()) {
    logger::info(s.path);
  }

  auto tex = registry.getTexture();
  for (const auto &t : tex.value()) {
    logger::info(t.path);
  }

  repo.deleteSelectedRow(1).or_else(logger::error);

  return 0;
}
