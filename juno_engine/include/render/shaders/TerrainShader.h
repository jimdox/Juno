#pragma once
#include "render/shaders/Shader.h"


namespace juno{
class TerrainShader : public Shader
{
public:
    TerrainShader();
	TerrainShader(const std::string& filepath);
	virtual ~TerrainShader();
	
};
}