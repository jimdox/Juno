#pragma once
#include "pch.h"


namespace juno {

enum TextureType : unsigned char
{
	TX_DIFFUSE, TX_SPECULAR, TX_NORMAL, TX_HEIGHT_MAP
};

class Texture
{
public:
	Texture(const std::string& filepath, TextureType tx_type);
	Texture(const std::string& dir, TextureType tx_type, bool transp);
	virtual ~Texture();

	const std::string& getFilePath();
	TextureType getTexType();

	bool containsTransparency();
	void setTransparency(bool flag);

	unsigned int getID();


	//Texture(const Texture& other);
	Texture& operator=(const Texture& other);
	

protected:
	Texture(TextureType tx_type);
	unsigned int textureID;
	bool transparency;
	const std::string& filepath;
	TextureType tex_type;




	};
}


