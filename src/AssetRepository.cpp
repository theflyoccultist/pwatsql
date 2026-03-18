#include <AssetRepository.hpp>
#include <Logs.hpp>
#include <Statement.hpp>

#include <format>
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
  stmt.reset();

  logger::info("Table created");

  return ResultT<Unit>::ok(Unit{});
}

const int type_idx = 1;
const int path_idx = 2;
const int lastmodified_idx = 3;
const int tags_idx = 4;
const int id_idx = 5;

ResultT<Unit> AssetRepository::insertData(const NewAsset &asset) {
  auto stmt_result = Statement::prepare(
      db_.get(), "INSERT INTO ASSETS (TYPE, PATH, LAST_MODIFIED, TAGS) "
                 "VAlUES (?, ?, ?, ?)");
  if (!stmt_result) {
    logger::error(stmt_result.error());
    return ResultT<Unit>::err(DbError::PrepareFailed);
  }

  Statement stmt = std::move(stmt_result.value());

  stmt.bind_text(type_idx, asset.type).or_else(logger::error);
  stmt.bind_text(path_idx, asset.path).or_else(logger::error);
  stmt.bind_int64(lastmodified_idx, asset.last_modified).or_else(logger::error);
  stmt.bind_text(tags_idx, asset.tags).or_else(logger::error);

  stmt.step().or_else(logger::error);
  stmt.reset();

  logger::info(std::format("Asset inserted: {}", asset.type, asset.path));

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

  stmt.bind_text(type_idx, asset.type).or_else(logger::error);
  stmt.bind_text(path_idx, asset.path).or_else(logger::error);
  stmt.bind_int64(lastmodified_idx, asset.last_modified).or_else(logger::error);
  stmt.bind_text(tags_idx, asset.tags).or_else(logger::error);
  stmt.bind_int(id_idx, asset.id).or_else(logger::error);

  stmt.step().or_else(logger::error);
  stmt.reset();

  logger::info(std::format("Asset updated: id={}", asset.id));

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
  stmt.reset();

  logger::info(std::format("Asset deleted: id={}", id));

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

  stmt.reset();

  logger::info(std::format("Asset retrieved: id={}", asset.id));

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

  stmt.reset();

  logger::info(std::format("Retrieved {} assets from database", result.size()));

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
