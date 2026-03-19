#include <AssetRegistry.hpp>
#include <vector>

ResultT<std::vector<MusicAsset>> AssetRegistry::getMusic() {
  return getAssetT<MusicAsset>("music");
}

ResultT<std::vector<SoundAsset>> AssetRegistry::getSfx() {
  return getAssetT<SoundAsset>("sfx");
}

ResultT<std::vector<TextureAsset>> AssetRegistry::getTexture() {
  return getAssetT<TextureAsset>("texture");
}
