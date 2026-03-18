#include <Logs.hpp>
#include <iostream>

LogLevel logger::current_level = LogLevel::Error;

void logger::set_level(LogLevel level) { current_level = level; }

void logger::error(const DbError &msg) {
  std::cerr << "[ERROR] " << msg << "\n";
}

void logger::info(const std::string &msg) {
  if (current_level < LogLevel::Info) {
    return;
  }

  std::cout << "[INFO] " << msg << "\n";
}
