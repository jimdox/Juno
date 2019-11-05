#include "render/textures/CubeMap.h"
#include "core/AssetLoader.h"
using namespace bx;

CubeMap::CubeMap(std::array<std::string, 6> texturePaths, TextureType txType) : Texture(txType), filepaths(texturePaths)
{
    textureID = bxImport::loadCubeMap(filepaths, GL_TEXTURE_CUBE_MAP, txType);

}

CubeMap::~CubeMap()
{

}

std::array<std::string, 6>& CubeMap::getFilepaths()
{
    return this->filepaths;
}