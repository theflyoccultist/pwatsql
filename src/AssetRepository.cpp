#include <AssetRepository.hpp>
#include <Logs.hpp>
#include <Statement.hpp>

#include <format>
#include <string>
#include <utility>
#include <vector>

std::string AssetRepository::createTableSQL(const Table &t) {
  std::string sql = "CREATE TABLE IF NOT EXISTS " + t.name + " (";

  for (size_t i = 0; i < t.columns.size(); ++i) {
    const auto &col = t.columns[i];
    sql += col.name + " " + col.type;

    if (i != t.columns.size() - 1) {
      sql += ", ";
    }
  }

  sql += ");";
  Logger::info(std::format("SQL: {}", sql));
  return sql;
}

ResultT<Unit> AssetRepository::createTable(const Table &table) {
  std::string sql = createTableSQL(table);

  auto stmt_result = Statement::prepare(db_.get(), sql.c_str());
  if (!stmt_result) {
    Logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.step().or_else(Logger::error);
  stmt.reset();

  return ResultT<Unit>::ok(Unit{});
}

std::string AssetRepository::insertDataSQL(const InsertRow &row) {
  std::string sql = "INSERT INTO " + row.tableName + " (";

  for (size_t i = 0; i < row.values.size(); ++i) {
    const auto &col = row.values[i].first;
    sql += col + " ";

    if (i != row.values.size() - 1) {
      sql += ", ";
    }
  }

  sql += ") VALUES (";

  for (size_t i = 0; i < row.values.size(); ++i) {
    sql += "? ";

    if (i != row.values.size() - 1) {
      sql += ", ";
    }
  }

  sql += ") ";
  Logger::info(std::format("SQL: {}", sql));
  return sql;
}

ResultT<Unit> AssetRepository::insertData(const InsertRow &row) {
  std::string sql = insertDataSQL(row);

  auto stmt_result = Statement::prepare(db_.get(), sql.c_str());
  if (!stmt_result) {
    Logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  for (int i = 0; i < row.values.size(); ++i) {
    stmt.bind(i + 1, row.values[i].second);
  }

  stmt.step().or_else(Logger::error);
  stmt.reset();

  return ResultT<Unit>::ok(Unit{});
}

std::string AssetRepository::updateDataSQL(const UpdateRow &row) {
  std::string sql = "UPDATE " + row.tableName + " SET ";

  for (size_t i = 0; i < row.values.size(); ++i) {
    const auto &col = row.values[i].first;
    sql += col + " = ? ";

    if (i != row.values.size() - 1) {
      sql += ", ";
    }
  }

  sql += "WHERE ID = ?";

  Logger::info(std::format("SQL: {}", sql));
  return sql;
}

ResultT<Unit> AssetRepository::updateData(const UpdateRow &row) {
  std::string sql = updateDataSQL(row);

  auto stmt_result = Statement::prepare(db_.get(), sql.c_str());
  if (!stmt_result) {
    Logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  for (int i = 0; i < row.values.size(); ++i) {
    stmt.bind(i + 1, row.values[i].second);
  }

  stmt.bind(static_cast<int>(row.values.size() + 1), row.id);

  stmt.step().or_else(Logger::error);
  stmt.reset();

  return ResultT<Unit>::ok(Unit{});
}

std::string AssetRepository::deleteRowSQL(const DeleteRow &row) {
  std::string sql = "DELETE FROM " + row.tableName + " WHERE ID = ?";
  Logger::info(std::format("SQL: {}", sql));
  return sql;
}

ResultT<Unit> AssetRepository::deleteRow(const DeleteRow &row) {
  std::string sql = deleteRowSQL(row);

  auto stmt_result = Statement::prepare(db_.get(), sql.c_str());
  if (!stmt_result) {
    Logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind(1, row.id);

  stmt.step().or_else(Logger::error);
  stmt.reset();

  return ResultT<Unit>::ok(Unit{});
}

std::string AssetRepository::selectRowSQL(const SelectQuery &row) {
  std::string sql = "SELECT ";
  for (size_t i = 0; i < row.columns.size(); ++i) {
    sql += row.columns[i];
    if (i != row.columns.size() - 1) {
      sql += ", ";
    }
  }

  sql += " FROM " + row.tableName + " WHERE " + row.whereColumn + " = ?";

  Logger::info(std::format("SQL: {}", sql));
  return sql;
}

ResultT<std::vector<std::vector<Value>>>
AssetRepository::getWhere(const SelectQuery &row) {
  std::string sql = selectRowSQL(row);

  auto stmt_result = Statement::prepare(db_.get(), sql.c_str());

  if (!stmt_result) {
    Logger::error(stmt_result.error());
    return ResultT<std::vector<std::vector<Value>>>::err(
        DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind(1, row.query);

  std::vector<std::vector<Value>> result;

  for (;;) {
    auto step = stmt.step();

    if (!step) {
      Logger::error(step.error());
      return ResultT<std::vector<std::vector<Value>>>::err(DbError::StepFailed);
    }

    if (!step.value()) {
      break;
    }

    std::vector<Value> entry;
    entry.reserve(row.columns.size());

    for (int i = 0; i < row.columns.size(); ++i) {
      entry.emplace_back(stmt.column(i));
    }

    result.push_back(std::move(entry));
  }

  return ResultT<std::vector<std::vector<Value>>>::ok(result);
}
