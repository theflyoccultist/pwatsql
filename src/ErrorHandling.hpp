#pragma once

#include <algorithm>
#include <cassert>
#include <utility>
#include <variant>

struct Unit {};

template <typename T, typename E> class [[nodiscard]] Result {
public:
  static Result ok(T value) { return Result(std::move(value)); }

  static Result err(E error) { return Result(std::move(error)); }

  [[nodiscard]] bool has_value() const noexcept {
    return std::holds_alternative<T>(data_);
  }

  explicit operator bool() const noexcept { return has_value(); }

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
    if (has_value()) {
      return f(value());
    }
    return decltype(f(value()))::err(error());
  }

  template <typename F> void or_else(F &&f) const {
    if (!has_value()) {
      std::forward<F>(f)(error());
    }
  }

private:
  std::variant<T, E> data_;

  Result(T value) : data_(std::move(value)) {}
  Result(E error) : data_(std::move(error)) {}
};
