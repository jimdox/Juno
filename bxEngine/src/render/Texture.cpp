#include "render/Texture.h"
#include "core/Log.h"
#include "stb_image.h"
#include <iostream>
using namespace bbx;

/* this constructor loads the texture itself */
Texture::Texture(std::string& filepath, std::string& texType, bool hasUVs) : filepath(filepath), texType(texType)
{
	int width, height, channels;
	texType = "diffuse";
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned char* imageData = stbi_load((filepath.c_str()), &width, &height, &channels, 0);

	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		BBX_CLI_INFO("Loaded" + this->filepath);
	}
	else
	{
		BBX_CLI_INFO("Failed to load image: " + filepath);
	}
	stbi_image_free(imageData);

}

/* called by bxAssetLoader */
Texture::Texture(std::string& filepath, std::string& texType) : filepath(filepath), texType(texType)
{	
	int width, height, channels;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	unsigned char* imageData = stbi_load((filepath.c_str()), &width, &height, &channels, 0);

	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		BBX_CLI_INFO(("Loaded Texture from location: " + filepath).c_str());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	}
	else
	{
		BBX_CLI_INFO("Failed to load image: " + filepath);
	}
	stbi_image_free(imageData);
}

Texture::~Texture()
{

}

GLuint Texture::getID()
{
	return this->textureID;
}

std::string& Texture::getFilePath()
{
	return filepath;
}

std::string& Texture::getTexType()
{
	return texType;
}

float Texture::getReflectivity()
{
	return this->reflectivity;
}

float Texture::getShineDamper()
{
	return this->shineDamper;
}

void Texture::setReflectivity(float& r)
{
	this->reflectivity = r;
}

void Texture::setShineDamper(float& sd)
{
	this->shineDamper = sd;
}


/*
Texture::Texture(const Texture& other)  : filepath(other.getFilePath()), texType(other.getTexType())// copy const.
{

}
*/

Texture& Texture::operator=(const Texture& rhs)  // assignment op
{
	if (this == &rhs) return *this;
	//assignment operator
	return *this;
}
