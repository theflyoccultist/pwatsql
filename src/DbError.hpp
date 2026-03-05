#pragma once

#include <cstdint>
#include <ostream>

enum class DbError : std::uint8_t {
  OpenFailed,
  PrepareFailed,
  BindFailed,
  RowReadFailed,
  ColumnReadFailed,
  TransactionError,
  ConstraintError,
  NotFound,
  Unknown
};

inline std::ostream &operator<<(std::ostream &os, const DbError &err) {
  switch (err) {
  case DbError::OpenFailed:
    os << "sqlite error: open failed";
    break;
  case DbError::PrepareFailed:
    os << "sqlite error: prepare failed";
    break;
  case DbError::BindFailed:
    os << "sqlite error: bind failed";
    break;
  case DbError::ColumnReadFailed:
    os << "sqlite error: column read failed";
    break;
  case DbError::RowReadFailed:
    os << "sqlite error: row read failed";
    break;
  case DbError::TransactionError:
    os << "sqlite error: transaction error";
    break;
  case DbError::ConstraintError:
    os << "sqlite error: constraint error";
    break;
  case DbError::NotFound:
    os << "sqlite error: not found";
    break;
  case DbError::Unknown:
    os << "sqlite error: unknown error";
    break;
  }
  return os;
}
