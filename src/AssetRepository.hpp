#pragma once

#include <Asset.hpp>
#include <Database.hpp>
#include <DbError.hpp>
#include <ErrorHandling.hpp>

#include <string_view>
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

  Result<Asset, DbError> getAssetById(int id);
  Result<std::vector<Asset>, DbError> getAssetsByType(std::string_view type);
  Result<std::vector<Asset>, DbError> getAssetsByTag(std::string_view tag);

  Result<std::vector<Asset>, DbError> getAllAssets();

private:
  Database &db_;
};
