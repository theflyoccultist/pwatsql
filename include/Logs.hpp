#pragma once

#include <Asset.hpp>
#include <DbError.hpp>
#include <cstdint>
#include <string>

enum class LogLevel : std::uint8_t {
  Error,
  Info,
};

class Logger {
public:
  static void set_level(LogLevel level);

  static void error(const DbError &msg);

  static void info(const std::string &msg);

private:
  static LogLevel current_level;
};
