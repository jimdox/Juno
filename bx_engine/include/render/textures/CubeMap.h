#pragma once
#include "render/textures/Texture.h"
#include <string>
#include <array>

namespace bx {
class CubeMap : Texture
{
public:
    CubeMap(std::array<std::string, 6> texturePaths, TextureType txType);
    ~CubeMap();

    std::array<std::string, 6>& getFilepaths();


private:
    std::array<std::string, 6> filepaths;

};

}