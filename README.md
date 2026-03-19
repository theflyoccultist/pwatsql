# PwatSQL: An SQLite3 wrapper in C++20

This project was created because I wanted to try using the SQLite3 API, and have a set of functions that I could reuse later when I'll need a database for my C++ projects.

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
Just a dumb layer for making generic CRUD operations to the sqlite3 API. That's actually where most of the sqlite3 magic and the SQL statements are implemented.

## Example Program
On Linux, you can simply build the library using the `./Build.sh` script, and run `build/example_basic -v` to see it working.

## Usage

#### Add Verbose Mode:

```cpp
#include <pwatsql.hpp>

int main(int argc, char **argv) {
  bool verbose = false;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[1];

    if (arg == "-v" || arg == "--verbose") {
      verbose = true;
    }
  }
}

  verbose ? Logger::set_level(LogLevel::Info)
          : Logger::set_level(LogLevel::Error);

```

#### Quick Start:

```cpp
  auto db_result = Database::open("db.sqlite3");
  if (!db_result) {
    Logger::error(db_result.error());
    return 1;
}

  Database db = std::move(db_result.value());
  AssetRepository repo(db);
  Transaction txn(db);

  repo.createTable().or_else(Logger::error);

  // Some asset actions

  txn.commit();
```

#### Inserting Assets:

```cpp
  const auto now = std::chrono::system_clock::now();
  const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

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
```

#### Updating Assets:

```cpp
  repo.updateData({.id = 2,
                   .type = "sfx",
                   .path = "sfx/angelic.ogg",
                   .last_modified = t_c,
                   .tags = "ally,luck"})
      .or_else(Logger::error);
```

#### Querying Assets:

```cpp
  auto music = repo.getAssetsByType("music");
  for (const auto &m : music.value()) {
    Logger::info(m.path);
  }
```

#### Deleting Assets:

```cpp
  repo.deleteSelectedRow(1).or_else(Logger::error);
```

Feel free to add caching and some Lua scripting for example, so you can benefit from performance and actually loading assets without hardcoding anything onto C++. Those are outside the scope of this small library, but perfect for an actual game engine.
