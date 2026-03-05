#pragma once

#include <DbError.hpp>
#include <iostream>

namespace {
inline void log_error(const DbError &msg) {
  std::cerr << "[ERROR] " << msg << "\n";
}

inline void log_info(const auto &msg) { std::cout << "[INFO] " << msg << "\n"; }
} // namespace
