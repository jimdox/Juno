#include "jnpch.h"
#include "Renderer/RenderObjects/Textures/CubeMap.h"
#include "Core/AssetManager.h"
using namespace Juno;

CubeMap::CubeMap() 
{
   // textureID = 0; //loadCubeMap(DEFAULT_CUBE_MAP_PATH, TX_DIFFUSE);
}

CubeMap::CubeMap(std::array<std::string, 6>& texturePaths, TextureType txType) //: Texture(txType), filepaths(texturePaths)
{
    //textureID = AssetManager::Get().LoadCubeMapFiles(texturePaths, txType);

}

CubeMap::~CubeMap()
{

}

std::array<std::string, 6>& CubeMap::GetFilepaths()
{
    return this->filepaths;
}