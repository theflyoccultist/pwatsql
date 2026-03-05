#pragma once

#include <Asset.hpp>
#include <Database.hpp>
#include <DbError.hpp>
#include <ErrorHandling.hpp>

#include <vector>

class AssetRepository {
public:
  explicit AssetRepository(Database &db) : db_(db) {}

  AssetRepository(const AssetRepository &) = delete;
  AssetRepository &operator=(const AssetRepository &) = delete;

  AssetRepository(const AssetRepository &&) = delete;
  AssetRepository &operator=(const AssetRepository &&) = delete;

  ~AssetRepository() = default;

  void createTable();
  void insertData(const Asset &asset);
  void deleteSelectedRow(int id);
  void updateData(const Asset &asset);

  Result<Asset, DbError> getSelectedRow(int id);
  std::vector<Asset> getAllRows();

private:
  Database &db_;
};
