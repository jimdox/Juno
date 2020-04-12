#pragma once
#include "render/textures/Texture.h"
#include <string>
#include <array>

namespace juno {
class CubeMap : public Texture
{
public:
    CubeMap();
    CubeMap(std::array<std::string, 6>& texturePaths, TextureType txType);
    ~CubeMap();

    std::array<std::string, 6>& getFilepaths();
    

private:
    std::array<std::string, 6> filepaths;

    const std::array<std::string, 6> DEFAULT_CUBE_MAP_PATH = 
    {
        "./juno_engine/res/red.png",
        "./juno_engine/res/red.png",
        "./juno_engine/res/red.png",
        "./juno_engine/res/red.png",
        "./juno_engine/res/red.png",
        "./juno_engine/res/red.png"
    };

};

}