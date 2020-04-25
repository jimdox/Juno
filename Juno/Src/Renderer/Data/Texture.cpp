#include "jnpch.h"
#include "Renderer/Data/Texture.h"
#include "Renderer/Renderer.h"

using namespace Juno;

SPtr<Texture> Texture::Create(TextureType texType, bool transparency)
{
    if(Renderer::Get().GetCurrentGraphicsAPI() == GraphicsAPI::Vulkan)
    {
        return std::make_shared<VKTexture>(texType, transparency);
    } else {
        return std::make_shared<GLTexture>(texType, transparency);
    }
}

GLTexture::GLTexture(TextureType texType, bool transparency) 
{
    this->textureType = texType;
    this->containsTransparency = transparency;
	glGenTextures(1, &textureID);
}

GLTexture::~GLTexture()
{

}

void GLTexture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void GLTexture::Unbind()
{

}

void GLTexture::Load(unsigned char** imageData, int width, int height)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (*imageData));
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(textureType == TextureType::Diffuse || textureType == TextureType::Specular)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // add anisotropic filtering
    } 

}

/* ---------- */

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
