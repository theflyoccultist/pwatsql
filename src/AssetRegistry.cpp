#include <AssetRegistry.hpp>
#include <DbError.hpp>
#include <ErrorHandling.hpp>
#include <vector>

ResultT<std::vector<MusicAsset>> AssetRegistry::getMusic() {
  auto result = repo_.getAssetsByType("music");
  if (!result) {
    return ResultT<std::vector<MusicAsset>>::err(DbError::NotFound);
  }

  std::vector<MusicAsset> music;

  for (const auto &asset : result.value()) {
    music.push_back(MusicAsset{
        .path = asset.path,
    });
  }

  return ResultT<std::vector<MusicAsset>>::ok(music);
}

ResultT<std::vector<SoundEffect>> AssetRegistry::getSfx() {
  auto result = repo_.getAssetsByType("sfx");
  if (!result) {
    return ResultT<std::vector<SoundEffect>>::err(DbError::NotFound);
  }

  std::vector<SoundEffect> sfx;

  for (const auto &asset : result.value()) {
    sfx.push_back(SoundEffect{
        .path = asset.path,
    });
  }

  return ResultT<std::vector<SoundEffect>>::ok(sfx);
}

ResultT<std::vector<TextureAsset>> AssetRegistry::getTexture() {
  auto result = repo_.getAssetsByType("texture");
  if (!result) {
    return ResultT<std::vector<TextureAsset>>::err(DbError::NotFound);
  }

  std::vector<TextureAsset> texture;

  for (const auto &asset : result.value()) {
    texture.push_back(TextureAsset{
        .path = asset.path,
    });
  }

  return ResultT<std::vector<TextureAsset>>::ok(texture);
}
