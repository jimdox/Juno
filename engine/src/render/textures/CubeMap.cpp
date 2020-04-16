#include "render/textures/CubeMap.h"
#include "core/AssetManager.h"
using namespace juno;

CubeMap::CubeMap() : Texture(TX_DIFFUSE)
{
    textureID = 0; //loadCubeMap(DEFAULT_CUBE_MAP_PATH, TX_DIFFUSE);
}

CubeMap::CubeMap(std::array<std::string, 6>& texturePaths, TextureType txType) : Texture(txType), filepaths(texturePaths)
{
    textureID = AssetManager::get().loadCubeMapFiles(texturePaths, txType);

}

CubeMap::~CubeMap()
{

}

std::array<std::string, 6>& CubeMap::getFilepaths()
{
    return this->filepaths;
}