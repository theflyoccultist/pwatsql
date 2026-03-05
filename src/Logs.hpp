#pragma once

#include <iostream>

void log_error(const auto &msg) { std::cerr << "[ERROR] " << msg << "\n"; }

void log_info(const auto &msg) { std::cout << "[INFO] " << msg << "\n"; }
