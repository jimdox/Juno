#pragma once

namespace Juno {

enum class TextureType : unsigned char
{
	DIFFUSE, SPECULAR, NORMAL, HEIGHT_MAP
};

class Texture
{
public:
	Texture(const std::string& filepath, TextureType texType);
	Texture(const std::string& filepath, TextureType texType, bool transp);
	virtual ~Texture();

	const std::string& GetFilePath();
	TextureType GetTextureType();

	bool ContainsTransparency();
	void SetTransparency(bool flag);

	unsigned int GetID();


	//Texture(const Texture& other);
	Texture& operator=(const Texture& other) = delete;
	

protected:
	Texture(TextureType texType);
	unsigned int textureID;
	bool transparency;
	TextureType textureType;

private:
	const std::string& filepath;

};
}


