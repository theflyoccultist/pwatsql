# PwatSQL: An SQLite3 wrapper library in C++20

This project was created because I wanted to try using the SQLite3 API, and have a set of functions that I could reuse later when I'll need a database for my C++ projects. Initially developed for asset loading, I then tried my best to generalize it so that it can be used in any kind of projects where a database might be needed. My objective was letting the user still be explicit with their SQL operations, just an utilitarian tool that doesn't try to be clever.

## Features:
- **Prepared statements**
This allows preventing SQL injections, and binding different variables to placeholders after compiling it only once. Pretty useful for Asset systems where similar queries might run many times over.
Prepared statements are basically like tiny programs, and the steps are as follows:

```
prepare → compile program
step → run program
reset → rewind program
finalize → destroy program
```

- **DB transactions**
This allows operations to either "commit" together, or "rollback" together. You definitely don't want statements to only be executed halfway through if something wrong happens.

- **Error handling using the `Result<T, Error>` pattern**
In my pwatgame project, I had made use of the `std::expected` feature, allowing me to have some elegant error handling when interfacing with the Lua API. An inconvenient with that was that it is only supported from C++23 onwards, meaning it wouldn't pass some CI tools that still uses C++20. To still keep that pattern I liked, while keeping compatibility with C++20, I made use of `std::variant`, which I wrapped with a set of small templates (present in the `ErrorHandling.hpp` file. I had also added `[[nodiscard]]` to the class to let the compiler inform me in case I forgot to handle some error cases, and added `and_then()` and `or_else()` to allow chaining some functions. Since `std::variant` doesn't accept `void` as a return value, My workaround was to return an empty struct.

- **AssetRepository.cpp and AssetRepository.hpp**
Just a small abstraction for making generic CRUD operations to the sqlite3 API, and adding a minimal type system. That's actually where most of the sqlite3 magic and the SQL statements are implemented, with the use of prepared SQL statements and bindings.

## Example Program
On Linux, you can simply build the library using the `./Build.sh` script, and run `build/example_basic -v` to see it working. You can also include this library in a project with CMake.

## Usage

An usage suggestion, this is how the DB layer for your project could look like. Of course, you are free to extend the functionalities of this library by using loops to insert data, or making use of a scripting interface, but this is a basic example.

```cpp
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
  Table assets = {.name = "ASSETS",
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
```
