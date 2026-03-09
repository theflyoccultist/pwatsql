# PwatSQL: An SQLite wrapper for asset loading in game engines

This project was created because I wanted to try using the SQLite3 API, and also because I wanted a way to load assets in my future game engines without recompilation. That had been one of the pain points I encountered while developing pwatgame, and this set of functions will hopefully be a way to remedy that for the future.

Features:
- Prepared statements
This allows preventing SQL injections, and binding different variables to placeholders after compiling it only once. Pretty useful for Asset systems where similar queries might run many times over.
Prepared statements are basically like tiny programs, and the steps are as follows:

```
prepare → compile program
step → run program
reset → rewind program
finalize → destroy program
```

- DB transactions
This allows operations to either "commit" together, or "rollback" together. You definitely don't want statements to only be executed halfway through if something wrong happens.

- Error handling using the `Result<T, Error>` pattern
In my pwatgame project, I had made use of the `std::expected` feature, allowing me to have some elegant error handling when interfacing with the Lua API. An inconvenient with that was that it is only supported from C++23 onwards, meaning it wouldn't pass some CI tools that still uses C++20. To still keep that pattern I liked, while keeping compatibility with C++20, I made use of `std::variant`, which I wrapped with a set of small templates (present in the `ErrorHandling.hpp` file. I had also added `[[nodiscard]]` to the class to let the compiler inform me in case I forgot to handle some error cases, and added `and_then()` and `or_else()` to allow chaining some functions. Since `std::variant` doesn't accept `void` as a return value, My workaround was to use an empty struct.
