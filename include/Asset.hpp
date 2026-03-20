#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>

struct Column {
  std::string name;
  std::string type;
};

struct Table {
  std::string name;
  std::vector<Column> columns;
};

using Value = std::variant<int64_t, double, std::string, std::nullptr_t>;

struct InsertRow {
  std::string tableName;
  std::vector<std::pair<std::string, Value>> values;
};

struct UpdateRow {
  std::string tableName;
  std::vector<std::pair<std::string, Value>> values;
  int64_t id;
};

struct DeleteRow {
  std::string tableName;
  int64_t id;
};

struct SelectQuery {
  std::string tableName;
  std::vector<std::string> columns;
  std::string whereColumn;
  Value query;
};
