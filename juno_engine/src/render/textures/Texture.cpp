#include "render/textures/Texture.h"
#include "core/AssetLoader.h"
// #include <GL/glew.h>
//#include "stb_image.h"
using namespace juno;

Texture::Texture(const std::string& filepath, TextureType tx_type) : filepath(filepath), tex_type(tex_type)
{
	transparency = false;
	textureID = juno::loadTexture(filepath, GL_TEXTURE_2D, tex_type);
}

Texture::Texture(const std::string& filepath, TextureType tx_type, bool transp) : filepath(filepath), tex_type(tx_type), transparency(transp)
{	
	textureID = juno::loadTexture(filepath, GL_TEXTURE_2D, tex_type);
}

Texture::Texture(TextureType tx_type) : filepath(""), tex_type(tx_type)
{
	transparency = false;

}

Texture::~Texture()
{

}

GLuint Texture::getID()
{
	return this->textureID;
}

bool Texture::containsTransparency()
{
	return transparency;
}

void Texture::setTransparency(bool flag)
{
	this->transparency = flag;
}

const std::string& Texture::getFilePath()
{
	return filepath;
}

TextureType Texture::getTexType()
{
	return tex_type;
}

/*
Texture::Texture(const Texture& other)  : filepath(other.getFilePath()), tex_type(other.gettex_type())// copy const.
{

}
*/

Texture& Texture::operator=(const Texture& rhs)  // assignment op
{
	if (this == &rhs) return *this;
	//assignment operator
	return *this;
}