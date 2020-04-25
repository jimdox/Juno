#pragma once
#include "Utils/Memory.h"
namespace Juno
{
enum class TextureType : unsigned char
{
	Diffuse, Specular, Normal, Displacement
};

class Texture
{
public:
    static SPtr<Texture> Create(TextureType texType, bool transparency);
    //virtual ~Texture(){}
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void Load(unsigned char** imageData, int width, int height) = 0;
    
protected:
    bool containsTransparency;
    TextureType textureType;
};
}


