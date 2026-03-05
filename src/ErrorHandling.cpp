#include <ErrorHandling.hpp>

std::ostream &operator<<(std::ostream &os, const DbError &err) {
  switch (err) {
  case DbError::OpenFailed:
    os << "sqlite error: open failed";
    break;
  case DbError::PrepareFailed:
    os << "sqlite error: prepare failed";
    break;
  case DbError::StepFailed:
    os << "sqlite error: step failed";
    break;
  case DbError::BindFailed:
    os << "sqlite error: bind failed";
    break;
  case DbError::RowReadFailed:
    os << "sqlite error: row read failed";
    break;
  case DbError::ConstraintViolation:
    os << "sqlite error: constraint violation";
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
