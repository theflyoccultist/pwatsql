#pragma once

#include <Asset.hpp>
#include <Database.hpp>
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

  ResultT<Unit> createTable();
  ResultT<Unit> insertData(const NewAsset &asset);
  ResultT<Unit> updateData(const AssetUpdate &asset);
  ResultT<Unit> deleteSelectedRow(int id);

  ResultT<Asset> getAssetById(int id);
  ResultT<std::vector<Asset>> getAssetsByType(std::string_view type);
  ResultT<std::vector<Asset>> getAssetsByTag(std::string_view tag);

  ResultT<std::vector<Asset>> getAllAssets();

private:
  Database &db_;
};
