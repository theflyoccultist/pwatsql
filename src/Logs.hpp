#pragma once

#include <iostream>

namespace {
void log_error(const auto &msg) { std::cerr << "[ERROR] " << msg << "\n"; }
void log_info(const auto &msg) { std::cout << "[INFO] " << msg << "\n"; }

inline auto error_msg() {
  return [](auto &e) { return log_error(e); };
}
} // namespace
