#pragma once

#include <AssetRepository.hpp>
#include <ErrorHandling.hpp>
#include <string>
#include <vector>

struct MusicAsset {
  std::string path;
};

struct SoundEffect {
  std::string path;
};

struct TextureAsset {
  std::string path;
};

class AssetRegistry {
public:
  explicit AssetRegistry(AssetRepository &repo) : repo_(repo) {}

  ResultT<std::vector<MusicAsset>> getMusic();
  ResultT<std::vector<SoundEffect>> getSfx();
  ResultT<std::vector<TextureAsset>> getTexture();

private:
  AssetRepository &repo_;
};
