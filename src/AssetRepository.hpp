#pragma once

#include "Asset.hpp"
#include "Database.hpp"
#include <optional>
#include <vector>

class AssetRepository {
public:
  explicit AssetRepository(Database &db) : db_(db) {}
  void createTable();
  void insertData(const Asset &asset);
  void deleteSelectedRow(int id);
  void updateData(const Asset &asset);

  std::optional<Asset> getSelectedRow(int id);
  std::vector<Asset> getAllRows();

private:
  Database &db_;
};
