#include <Logs.hpp>
#include <iostream>
#include <variant>

LogLevel Logger::current_level = LogLevel::Error;

void Logger::set_level(LogLevel level) { current_level = level; }

void Logger::error(const DbError &msg) {
  std::cerr << "[ERROR] " << msg << "\n";
}

void Logger::info(const Value &msg) {
  if (current_level < LogLevel::Info) {
    return;
  }

  std::visit([](auto &&v) { std::cout << "[INFO]: " << v << "\n"; }, msg);
}
