#include "jnpch.h"
#include "Renderer/Textures/Texture.h"
#include "Core/AssetManager.h"
// #include <GL/glew.h>
//#include "stb_image.h"
using namespace Juno;

Texture::Texture(const std::string& filepath, TextureType texType) : filepath(filepath), textureType(texType)
{
	transparency = false;
	textureID = AssetManager::Get().LoadTextureFile(filepath, GL_TEXTURE_2D, texType);
}

Texture::Texture(const std::string& filepath, TextureType texType, bool transp) : filepath(filepath), textureType(texType), transparency(transp)
{	
	textureID = AssetManager::Get().LoadTextureFile(filepath, GL_TEXTURE_2D, texType);
}

Texture::Texture(TextureType texType) : filepath(" "), textureType(texType)
{
	transparency = false;

}

Texture::~Texture()
{
	
}

GLuint Texture::GetID()
{
	return this->textureID;
}

bool Texture::ContainsTransparency()
{
	return transparency;
}

void Texture::SetTransparency(bool flag)
{
	this->transparency = flag;
}

const std::string& Texture::GetFilePath()
{
	return filepath;
}

TextureType Texture::GetTextureType()
{
	return textureType;
}

/*
Texture::Texture(const Texture& other)  : filepath(other.getFilePath()), tex_type(other.gettex_type())// copy const.
{

}
*/

// Texture& Texture::operator=(const Texture& rhs)  // assignment op
// {
// 	if (this == &rhs) return *this;
// 	//assignment operator
// 	return *this;
// }