#include <AssetRepository.hpp>
#include <Logs.hpp>
#include <Statement.hpp>

#include <string>
#include <string_view>
#include <utility>
#include <vector>

using std::vector;

ResultT<Unit> AssetRepository::createTable() {
  auto stmt_result =
      Statement::prepare(db_.get(), "CREATE TABLE IF NOT EXISTS ASSETS ("
                                    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                    "TYPE TEXT NOT NULL,"
                                    "PATH TEXT NOT NULL UNIQUE,"
                                    "LAST_MODIFIED INTEGER NOT NULL,"
                                    "TAGS TEXT)");
  if (!stmt_result) {
    logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.step().or_else(logger::error);
  return ResultT<Unit>::ok(Unit{});
}

ResultT<Unit> AssetRepository::insertData(const NewAsset &asset) {
  auto stmt_result = Statement::prepare(
      db_.get(), "INSERT INTO ASSETS (TYPE, PATH, LAST_MODIFIED, TAGS) "
                 "VAlUES (?, ?, ?, ?)");
  if (!stmt_result) {
    logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_text(1, asset.type).or_else(logger::error);
  stmt.bind_text(2, asset.path).or_else(logger::error);
  stmt.bind_int64(3, asset.last_modified).or_else(logger::error);
  stmt.bind_text(4, asset.tags).or_else(logger::error);

  stmt.step().or_else(logger::error);
  return ResultT<Unit>::ok(Unit{});
}

ResultT<Unit> AssetRepository::updateData(const AssetUpdate &asset) {
  auto stmt_result = Statement::prepare(
      db_.get(),
      "UPDATE ASSETS SET TYPE = ?, PATH = ?, LAST_MODIFIED = ?, TAGS = ? "
      "WHERE ID = ?");
  if (!stmt_result) {
    logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_text(1, asset.type).or_else(logger::error);
  stmt.bind_text(2, asset.path).or_else(logger::error);
  stmt.bind_int64(3, asset.last_modified).or_else(logger::error);
  stmt.bind_text(4, asset.tags).or_else(logger::error);
  stmt.bind_int(5, asset.id).or_else(logger::error);

  stmt.step().or_else(logger::error);
  return ResultT<Unit>::ok(Unit{});
}

ResultT<Unit> AssetRepository::deleteSelectedRow(int id) {
  auto stmt_result =
      Statement::prepare(db_.get(), "DELETE FROM ASSETS WHERE ID = ?");
  if (!stmt_result) {
    logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_int(1, id).or_else(logger::error);

  stmt.step().or_else(logger::error);
  return ResultT<Unit>::ok(Unit{});
}

ResultT<Asset> AssetRepository::getAssetById(int id) {
  auto stmt_result = Statement::prepare(
      db_.get(), "SELECT TYPE, PATH, LAST_MODIFIED, TAGS FROM ASSETS "
                 "WHERE ID = ?");

  if (!stmt_result) {
    logger::error(stmt_result.error());
    return Result<Asset, DbError>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_int(1, id).or_else(logger::error);
  stmt.step().or_else(logger::error);

  Asset asset;
  asset.id = id;
  asset.type = stmt.column_text(0);
  asset.path = stmt.column_text(1);
  asset.last_modified = stmt.column_int64(2);
  asset.tags = stmt.column_text(3);

  return Result<Asset, DbError>::ok(asset);
}

namespace {
ResultT<vector<Asset>> getAssets(Statement &stmt, vector<Asset> &result) {
  for (;;) {
    auto step = stmt.step();

    if (!step) {
      logger::error(step.error());
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

  return Result<vector<Asset>, DbError>::ok(result);
}
} // namespace

ResultT<vector<Asset>> AssetRepository::getAssetsByType(std::string_view type) {
  auto stmt_result =
      Statement::prepare(db_.get(), "SELECT * FROM ASSETS WHERE TYPE = ? ");
  if (!stmt_result) {
    logger::error(stmt_result.error());
    return Result<vector<Asset>, DbError>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_text(1, std::string(type)).or_else(logger::error);
  vector<Asset> result{};

  return getAssets(stmt, result);
}

ResultT<vector<Asset>> AssetRepository::getAssetsByTag(std::string_view tag) {
  auto stmt_result = Statement::prepare(
      db_.get(), "SELECT * FROM ASSETS WHERE TAGS LIKE '%' || ? || '%' ");
  if (!stmt_result) {
    logger::error(stmt_result.error());
    return Result<vector<Asset>, DbError>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_text(1, std::string(tag)).or_else(logger::error);
  vector<Asset> result{};

  return getAssets(stmt, result);
}

ResultT<vector<Asset>> AssetRepository::getAllAssets() {
  auto stmt_result = Statement::prepare(
      db_.get(), "SELECT ID, TYPE, PATH, LAST_MODIFIED, TAGS FROM ASSETS ");
  if (!stmt_result) {
    logger::error(stmt_result.error());
    return Result<vector<Asset>, DbError>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  vector<Asset> result{};

  return getAssets(stmt, result);
};
