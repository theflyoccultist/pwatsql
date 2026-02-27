#include "AssetRepository.hpp"
#include "Statement.hpp"
#include <optional>

void AssetRepository::createTable() {
  Statement stmt(db_.get(), "CREATE TABLE IF NOT EXISTS ASSETS ("
                            "ID INTEGER PRIMARY KEY,"
                            "TYPE TEXT NOT NULL,"
                            "PATH TEXT NOT NULL UNIQUE,"
                            "LAST_MODIFIED INTEGER NOT NULL,"
                            "TAGS TEXT)");

  stmt.step();
}

void AssetRepository::insertData(const Asset &asset) {
  Statement stmt(db_.get(),
                 "INSERT INTO ASSETS (TYPE, PATH, LAST_MODIFIED, TAGS) "
                 "VAlUES (?, ?, ?, ?)");

  stmt.bind(1, asset.type);
  stmt.bind(2, asset.path);
  stmt.bind(3, asset.last_modified);
  stmt.bind(4, asset.tags);

  stmt.step();
}

void AssetRepository::deleteSelectedRow(int id) {
  Statement stmt(db_.get(), "DELETE FROM ASSETS WHERE ID = ?");
  stmt.bind(1, id);

  stmt.step();
}

void AssetRepository::updateData(const Asset &asset) {
  Statement stmt(
      db_.get(),
      "UPDATE ASSETS SET TYPE = ?, PATH = ?, LAST_MODIFIED = ?, TAGS = ? "
      "WHERE ID = ?");

  stmt.bind(1, asset.type);
  stmt.bind(2, asset.path);
  stmt.bind(3, asset.last_modified);
  stmt.bind(4, asset.tags);
  stmt.bind(5, asset.id);

  stmt.step();
}

std::optional<Asset> AssetRepository::getSelectedRow(int id) {
  Statement stmt(db_.get(),
                 "SELECT TYPE, PATH, LAST_MODIFIED, TAGS FROM ASSETS "
                 "WHERE ID = ?");

  stmt.bind(1, id);
  if (!stmt.step()) {
    return std::nullopt;
  }

  Asset asset;
  asset.id = id;
  asset.type = stmt.column_text(0);
  asset.path = stmt.column_text(1);
  asset.last_modified = stmt.column_int64(2);
  asset.tags = stmt.column_text(3);

  return asset;
}

std::vector<Asset> AssetRepository::getAllRows() {
  Statement stmt(db_.get(),
                 "SELECT ID, TYPE, PATH, LAST_MODIFIED, TAGS FROM ASSETS ");

  std::vector<Asset> result;

  while (stmt.step()) {
    Asset asset;
    asset.id = stmt.column_int(0);
    asset.type = stmt.column_text(1);
    asset.path = stmt.column_text(2);
    asset.last_modified = stmt.column_int64(3);
    asset.tags = stmt.column_text(4);

    result.push_back(std::move(asset));
  }

  return result;
};
