#include <pwatsql.hpp>

#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>

// Basic example

int main(int argc, char **argv) {
  // Setup verbose mode
  bool verbose = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[1];

    if (arg == "-v" || arg == "--verbose") {
      verbose = true;
    }
  }

  verbose ? Logger::set_level(LogLevel::Info)
          : Logger::set_level(LogLevel::Error);

  const auto now = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

  // Start database
  auto db_result = Database::open("db.sqlite3");
  if (!db_result) {
    Logger::error(db_result.error());
    return 1;
  }

  Database db = std::move(db_result.value());
  AssetRepository repo(db);

  // Start DB Transaction
  Transaction txn(db);

  // Create table
  Table assets = {.tableName = "ASSETS",
                  .columns = {
                      {"ID", "INTEGER PRIMARY KEY"},
                      {"TYPE", "TEXT"},
                      {"PATH", "TEXT"},
                      {"LAST_MODIFIED", "INTEGER"},
                  }};

  repo.createTable(assets).or_else(Logger::error);

  // Insert row
  InsertRow row_1 = {.tableName = "ASSETS",
                     .values = {
                         {"TYPE", std::string("sfx")},
                         {"PATH", std::string("assets/combat.wav")},
                         {"LAST_MODIFIED", int64_t(t_c)},
                     }};

  repo.insertData(row_1).or_else(Logger::error);

  InsertRow row_2 = {.tableName = "ASSETS",
                     .values = {
                         {"TYPE", std::string("sfx")},
                         {"PATH", std::string("assets/angelic.wav")},
                         {"LAST_MODIFIED", int64_t(t_c)},
                     }};

  repo.insertData(row_2).or_else(Logger::error);

  // Commit DB Transaction
  txn.commit();

  // Update Row
  UpdateRow row_2_update = {
      .tableName = "ASSETS",
      .values =
          {
              {"TYPE", std::string("music")},
              {"PATH", std::string("assets/boogie.ogg")},
              {"LAST_MODIFIED", int64_t(t_c)},
          },
      .id = 2,
  };

  repo.updateData(row_2_update).or_else(Logger::error);

  // Select Row
  SelectQuery select_music = {
      .tableName = "ASSETS",
      .columns = {"PATH", "LAST_MODIFIED"},
      .whereColumn = "TYPE",
      .query = std::string("sfx"),
  };

  auto select = repo.getWhere(select_music);
  Logger::info("Selected row: ");
  for (const auto &entry : select.value()) {
    for (const auto &e : entry) {
      // Simply display row
      Logger::info(e);
    }
  }

  // Delete row
  DeleteRow row_2_delete = {
      .tableName = "ASSETS",
      .id = 2,
  };

  repo.deleteRow(row_2_delete).or_else(Logger::error);

  return 0;
}
