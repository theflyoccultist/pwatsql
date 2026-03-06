#pragma once

#include <DbError.hpp>

#include <cstdint>
#include <iostream>

enum class LogLevel : std::uint8_t {
  Error,
  Info,
};

struct Level {
  static LogLevel current_level;
};

namespace logger {
inline void set_level(LogLevel level) { Level::current_level = level; }

inline void error(const DbError &msg) {
  std::cerr << "[ERROR] " << msg << "\n";
}

inline void info(const auto &msg) {
  if (Level::current_level < LogLevel::Info) {
    return;
  }

  std::cout << "[INFO] " << msg << "\n";
}
} // namespace logger
