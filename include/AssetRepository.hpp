#pragma once

#include <Asset.hpp>
#include <Database.hpp>
#include <ErrorHandling.hpp>

class AssetRepository {
public:
  explicit AssetRepository(Database &db) : db_(db) {}

  AssetRepository(const AssetRepository &) = delete;
  AssetRepository &operator=(const AssetRepository &) = delete;

  AssetRepository(const AssetRepository &&) = delete;
  AssetRepository &operator=(const AssetRepository &&) = delete;

  ~AssetRepository() = default;

  ResultT<Unit> createTable(const Table &table);
  ResultT<Unit> insertData(const InsertRow &row);
  ResultT<Unit> updateData(const UpdateRow &row);
  ResultT<Unit> deleteRow(const DeleteRow &row);
  ResultT<std::vector<std::vector<Value>>> getWhere(const SelectQuery &row);

private:
  Database &db_;

  static std::string createTableSQL(const Table &t);
  static std::string insertDataSQL(const InsertRow &row);
  static std::string updateDataSQL(const UpdateRow &row);
  static std::string deleteRowSQL(const DeleteRow &row);
  static std::string selectRowSQL(const SelectQuery &row);
};
