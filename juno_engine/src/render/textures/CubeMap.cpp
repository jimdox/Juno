#include "render/textures/CubeMap.h"
#include "core/AssetLoader.h"
using namespace juno;

CubeMap::CubeMap() : Texture(TX_DIFFUSE)
{
    textureID = 0; //loadCubeMap(DEFAULT_CUBE_MAP_PATH, TX_DIFFUSE);
}

CubeMap::CubeMap(std::array<std::string, 6>& texturePaths, TextureType txType) : Texture(txType), filepaths(texturePaths)
{
    textureID = juno::loadCubeMap(texturePaths, txType);

}

CubeMap::~CubeMap()
{

}

std::array<std::string, 6>& CubeMap::getFilepaths()
{
    return this->filepaths;
}