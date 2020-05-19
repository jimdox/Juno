#include "jnpch.h"
#include "Renderer/Data/Texture.h"
#include "Renderer/Platforms/GLTexture.h"
#include "Renderer/Platforms/VKTexture.h"

using namespace Juno;

VKTexture::VKTexture(TextureType texType, bool transparency)
{
    this->textureType = texType;
    this->containsTransparency = transparency;
    
}

VKTexture::~VKTexture()
{
    
}

void VKTexture::Bind()
{

}

void VKTexture::Unbind()
{

}

void VKTexture::Load(unsigned char** imageData, int width, int height)
{

}