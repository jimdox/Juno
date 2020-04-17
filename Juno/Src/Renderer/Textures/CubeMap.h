#pragma once
#include "Renderer/Textures/Texture.h"
#include <string>
#include <array>

namespace Juno {
class CubeMap : public Texture
{
public:
    CubeMap();
    CubeMap(std::array<std::string, 6>& texturePaths, TextureType txType);
    ~CubeMap();

    std::array<std::string, 6>& GetFilepaths();
    

private:
    std::array<std::string, 6> filepaths;

    const std::array<std::string, 6> DEFAULT_CUBE_MAP_PATH = 
    {
        "./Resources/Textures/planet.png",
        "./Resources/Textures/planet.png",
        "./Resources/Textures/planet.png",
        "./Resources/Textures/planet.png",
        "./Resources/Textures/planet.png",
        "./Resources/Textures/planet.png",
    };

};

}