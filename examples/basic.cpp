
#include <chrono>
#include <ctime>
#include <pwatsql.hpp>
#include <string>

int main(int argc, char **argv) {
  bool verbose = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[1];

    if (arg == "-v" || arg == "--verbose") {
      verbose = true;
    }
  }

  verbose ? Logger::set_level(LogLevel::Info)
          : Logger::set_level(LogLevel::Error);

  // Test Program

  const auto now = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

  auto db_result = Database::open("db.sqlite3");
  if (!db_result) {
    Logger::error(db_result.error());
    return 1;
  }

  Database db = std::move(db_result.value());
  AssetRepository repo(db);

  Transaction txn(db);

  repo.createTable().or_else(Logger::error);

  repo.insertData({.type = "music",
                   .path = "music/loudboom.wav",
                   .last_modified = t_c,
                   .tags = "combat,menu"})
      .or_else(Logger::error);

  repo.insertData({.type = "sfx",
                   .path = "sfx/danger.ogg",
                   .last_modified = t_c,
                   .tags = "enemy,action"})
      .or_else(Logger::error);

  repo.insertData({.type = "texture",
                   .path = "texture/hero.png",
                   .last_modified = t_c,
                   .tags = "character,mesh"})
      .or_else(Logger::error);

  repo.updateData({.id = 2,
                   .type = "sfx",
                   .path = "sfx/angelic.ogg",
                   .last_modified = t_c,
                   .tags = "ally,luck"})
      .or_else(Logger::error);

  txn.commit();

  auto music = repo.getAssetsByType("music");
  for (const auto &m : music.value()) {
    Logger::info(m.path);
  }

  auto sfx = repo.getAssetsByType("sfx");
  for (const auto &s : sfx.value()) {
    Logger::info(s.path);
  }

  auto tex = repo.getAssetsByType("texture");
  for (const auto &t : tex.value()) {
    Logger::info(t.path);
  }

  repo.deleteSelectedRow(1).or_else(Logger::error);

  return 0;
}
