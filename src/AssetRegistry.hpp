#pragma once

#include <AssetRepository.hpp>
#include <AssetTypes.hpp>
#include <ErrorHandling.hpp>
#include <vector>

class AssetRegistry {
public:
  explicit AssetRegistry(AssetRepository &repo) : repo_(repo) {}

  AssetRegistry(const AssetRegistry &) = delete;
  AssetRegistry &operator=(const AssetRegistry &) = delete;

  AssetRegistry(const AssetRegistry &&) = delete;
  AssetRegistry &operator=(const AssetRegistry &&) = delete;

  ~AssetRegistry() = default;

  template <typename T>
  ResultT<std::vector<T>> getAssetT(std::string_view type);

  ResultT<std::vector<MusicAsset>> getMusic();
  ResultT<std::vector<SoundAsset>> getSfx();
  ResultT<std::vector<TextureAsset>> getTexture();

private:
  AssetRepository &repo_;
};

template <typename T>
ResultT<std::vector<T>> AssetRegistry::getAssetT(std::string_view type) {
  auto result = repo_.getAssetsByType(type);
  if (!result) {
    return ResultT<std::vector<T>>::err(DbError::NotFound);
  }

  std::vector<T> output;

  for (const auto &asset : result.value()) {
    output.push_back(T{
        .path = asset.path,
    });
  }

  return ResultT<std::vector<T>>::ok(output);
}
