#include "render/textures/CubeMap.h"
#include "core/AssetLoader.h"
using namespace juno;

CubeMap::CubeMap(std::array<const std::string, 6> texturePaths, TextureType txType) : Texture(txType), filepaths(texturePaths)
{
    textureID = juno::loadCubeMap(texturePaths, GL_TEXTURE_CUBE_MAP, txType);

}

CubeMap::~CubeMap()
{

}

std::array<const std::string, 6>& CubeMap::getFilepaths()
{
    return this->filepaths;
}