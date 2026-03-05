#include <AssetRepository.hpp>
#include <Statement.hpp>

#include <iostream>
#include <utility>

void AssetRepository::createTable() {
  auto stmt_result =
      Statement::prepare(db_.get(), "CREATE TABLE IF NOT EXISTS ASSETS ("
                                    "ID INTEGER PRIMARY KEY,"
                                    "TYPE TEXT NOT NULL,"
                                    "PATH TEXT NOT NULL UNIQUE,"
                                    "LAST_MODIFIED INTEGER NOT NULL,"
                                    "TAGS TEXT)");
  if (!stmt_result) {
    std::cerr << stmt_result.error() << "\n";
  }

  Statement stmt = std::move(stmt_result.value());
  stmt.step();
}

void AssetRepository::insertData(const Asset &asset) {
  auto stmt_result = Statement::prepare(
      db_.get(), "INSERT INTO ASSETS (TYPE, PATH, LAST_MODIFIED, TAGS) "
                 "VAlUES (?, ?, ?, ?)");
  if (!stmt_result) {
    std::cerr << stmt_result.error() << "\n";
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind(1, asset.type);
  stmt.bind(2, asset.path);
  stmt.bind(3, asset.last_modified);
  stmt.bind(4, asset.tags);

  stmt.step();
}

void AssetRepository::deleteSelectedRow(int id) {
  auto stmt_result =
      Statement::prepare(db_.get(), "DELETE FROM ASSETS WHERE ID = ?");
  if (!stmt_result) {
    std::cerr << stmt_result.error() << "\n";
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind(1, id);

  stmt.step();
}

void AssetRepository::updateData(const Asset &asset) {
  auto stmt_result = Statement::prepare(
      db_.get(),
      "UPDATE ASSETS SET TYPE = ?, PATH = ?, LAST_MODIFIED = ?, TAGS = ? "
      "WHERE ID = ?");
  if (!stmt_result) {
    std::cerr << stmt_result.error() << "\n";
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind(1, asset.type);
  stmt.bind(2, asset.path);
  stmt.bind(3, asset.last_modified);
  stmt.bind(4, asset.tags);
  stmt.bind(5, asset.id);

  stmt.step();
}

Result<Asset, DbError> AssetRepository::getSelectedRow(int id) {
  auto stmt_result = Statement::prepare(
      db_.get(), "SELECT TYPE, PATH, LAST_MODIFIED, TAGS FROM ASSETS "
                 "WHERE ID = ?");
  if (!stmt_result) {
    std::cerr << stmt_result.error() << "\n";
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind(1, id);
  if (!stmt.step()) {
    return Result<Asset, DbError>::err(DbError::RowReadFailed);
  }

  Asset asset;
  asset.id = id;
  asset.type = stmt.column_text(0);
  asset.path = stmt.column_text(1);
  asset.last_modified = stmt.column_int64(2);
  asset.tags = stmt.column_text(3);

  return Result<Asset, DbError>::ok(asset);
}

std::vector<Asset> AssetRepository::getAllRows() {
  auto stmt_result = Statement::prepare(
      db_.get(), "SELECT ID, TYPE, PATH, LAST_MODIFIED, TAGS FROM ASSETS ");
  if (!stmt_result) {
    std::cerr << stmt_result.error() << "\n";
  }

  Statement stmt = std::move(stmt_result.value());

  std::vector<Asset> result;

  for (;;) {
    auto step = stmt.step();

    if (!step) {
      std::cerr << step.error() << "\n";
      break;
    }

    if (!step.value())
      break;

    Asset asset{
        stmt.column_int(0),   stmt.column_text(1), stmt.column_text(2),
        stmt.column_int64(3), stmt.column_text(4),
    };

    result.push_back(std::move(asset));
  }

  return result;
};
