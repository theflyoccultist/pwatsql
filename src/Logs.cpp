#include <Logs.hpp>
#include <iostream>

LogLevel Level::current_level = LogLevel::Error;

void logger::set_level(LogLevel level) { Level::current_level = level; }

void logger::error(const DbError &msg) {
  std::cerr << "[ERROR] " << msg << "\n";
}

void logger::info(const std::string &msg) {
  if (Level::current_level < LogLevel::Info) {
    return;
  }

  std::cout << "[INFO] " << msg << "\n";
}

void logger::info(const Asset &msg) {
  if (Level::current_level < LogLevel::Info) {
    return;
  }

  std::cout << "[INFO] " << msg << "\n";
}
