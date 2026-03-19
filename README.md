# PwatSQL: An SQLite library for asset loading in game engines

This project was created because I wanted to try using the SQLite3 API, and also because I wanted a way to load assets in my future game engines without recompilation. That had been one of the pain points I encountered while developing pwatgame, and this set of functions will hopefully be a way to remedy that for the future.

Features:
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

- **AssetRegistry.cpp and AssetRegistry.hpp**
Honestly it's mostly a thin layer over `AssetRepository::getAssetByType()` to add some type safety over different types of assets. It is mostly a thing so cases such as adding the wrong type of texture (Texture as a SoundEffect asset type for example) can be avoided.

- **AssetCache.hpp**
Databases are made storing data persistently, but they aren't really made to be performant when doing repetitive calls. So, cache using Hash Maps are a thing. Hash Maps and Databases complement each other because one has speed and type safety, while the other has persistence. It's mostly a matter of milliseconds vs nanoseconds, but since I use C++ I like making speed demons when possible.

- **main.cpp**
At the moment it just exists to show example usages, and test my own stuff to see if it works as expected. Eventually I'm gonna add Lua so that assets can be loaded without recompiling the program, so that assets can be loaded just by writing a bit of Lua. There is also a toggle mode to enable verbose mode with `-v` or `--verbose`. Default mode only shows important errors, Verbose mode shows the work that sqlite3 does.
