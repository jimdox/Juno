#include "render/textures/CubeMap.h"

using namespace bx;

CubeMap::CubeMap(std::array<std::string, 6> texturePaths, TextureType txType) : Texture(txType), filepaths(texturePaths)
{


}

CubeMap::~CubeMap()
{

}

std::array<std::string, 6>& CubeMap::getFilepaths()
{
    return this->filepaths;
}