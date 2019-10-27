#pragma once
#include "render/shaders/Shader.h"


namespace bx{
class TerrainShader : public Shader
{
public:
    TerrainShader();
	TerrainShader(const std::string& filepath);
	virtual ~TerrainShader();
	
};
}