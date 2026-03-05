#pragma once

#include <algorithm>
#include <cassert>
#include <ostream>
#include <utility>
#include <variant>

enum class DbError {
  OpenFailed,
  PrepareFailed,
  StepFailed,
  BindFailed,
  RowReadFailed,
  ConstraintViolation,
  NotFound,
  Unknown
};

std::ostream &operator<<(std::ostream &os, const DbError &err);

struct Unit {};

template <typename T, typename E> class Result {
public:
  static Result ok(T value) { return Result(std::move(value)); }

  static Result err(E error) { return Result(std::move(error)); }

  bool has_value() const { return std::holds_alternative<T>(data_); }

  explicit operator bool() const { return has_value(); }

  T &value() {
    assert(has_value());
    return std::get<T>(data_);
  }

  const T &value() const {
    assert(has_value());
    return std::get<T>(data_);
  }

  E &error() {
    assert(!has_value());
    return std::get<E>(data_);
  }

  const E &error() const {
    assert(!has_value());
    return std::get<E>(data_);
  }

  template <typename F> auto and_then(F f) {
    if (has_value())
      return f(value());
    return decltype(f(value()))::err(error());
  }

private:
  std::variant<T, E> data_;

  Result(T value) : data_(std::move(value)) {}
  Result(E error) : data_(std::move(error)) {}
};
