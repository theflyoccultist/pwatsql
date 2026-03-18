#include <AssetRegistry.hpp>
#include <vector>

ResultT<std::vector<MusicAsset>> AssetRegistry::getMusic() {
  return getAssetT<MusicAsset>("music");
}

ResultT<std::vector<SoundEffect>> AssetRegistry::getSfx() {
  return getAssetT<SoundEffect>("sfx");
}

ResultT<std::vector<TextureAsset>> AssetRegistry::getTexture() {
  return getAssetT<TextureAsset>("texture");
}
