#include <AssetRepository.hpp>
#include <Logs.hpp>
#include <Statement.hpp>

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
    log_error(stmt_result.error());
    return;
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.step().or_else(error_msg());
}

void AssetRepository::insertData(const Asset &asset) {
  auto stmt_result = Statement::prepare(
      db_.get(), "INSERT INTO ASSETS (TYPE, PATH, LAST_MODIFIED, TAGS) "
                 "VAlUES (?, ?, ?, ?)");
  if (!stmt_result) {
    log_error(stmt_result.error());
    return;
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_text(1, asset.type).or_else(error_msg());
  stmt.bind_text(2, asset.path).or_else(error_msg());
  stmt.bind_int64(3, asset.last_modified).or_else(error_msg());
  stmt.bind_text(4, asset.tags).or_else(error_msg());

  stmt.step().or_else(error_msg());
}

void AssetRepository::deleteSelectedRow(int id) {
  auto stmt_result =
      Statement::prepare(db_.get(), "DELETE FROM ASSETS WHERE ID = ?");
  if (!stmt_result) {
    log_error(stmt_result.error());
    return;
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_int(1, id).or_else(error_msg());

  stmt.step().or_else(error_msg());
}

void AssetRepository::updateData(const Asset &asset) {
  auto stmt_result = Statement::prepare(
      db_.get(),
      "UPDATE ASSETS SET TYPE = ?, PATH = ?, LAST_MODIFIED = ?, TAGS = ? "
      "WHERE ID = ?");
  if (!stmt_result) {
    log_error(stmt_result.error());
    return;
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_text(1, asset.type).or_else(error_msg());
  stmt.bind_text(2, asset.path).or_else(error_msg());
  stmt.bind_int64(3, asset.last_modified).or_else(error_msg());
  stmt.bind_text(4, asset.tags).or_else(error_msg());
  stmt.bind_int(5, asset.id).or_else(error_msg());

  stmt.step().or_else(error_msg());
}

Result<Asset, DbError> AssetRepository::getSelectedRow(int id) {
  auto stmt_result = Statement::prepare(
      db_.get(), "SELECT TYPE, PATH, LAST_MODIFIED, TAGS FROM ASSETS "
                 "WHERE ID = ?");

  if (!stmt_result) {
    log_error(stmt_result.error());
    return Result<Asset, DbError>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_int(1, id).or_else(error_msg());
  stmt.step().or_else(error_msg());

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
    log_error(stmt_result.error());
    return {};
  }

  Statement stmt = std::move(stmt_result.value());

  std::vector<Asset> result{};

  for (;;) {
    auto step = stmt.step();

    if (!step) {
      log_error(step.error());
      break;
    }

    if (!step.value()) {
      break;
    }

    Asset asset{
        .id = stmt.column_int(0),
        .type = stmt.column_text(1),
        .path = stmt.column_text(2),
        .last_modified = stmt.column_int64(3),
        .tags = stmt.column_text(4),
    };

    result.push_back(std::move(asset));
  }

  return result;
};
