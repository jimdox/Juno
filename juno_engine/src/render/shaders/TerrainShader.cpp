#include "pch.h"
#include "render/shaders/TerrainShader.h"


using namespace juno;

TerrainShader::TerrainShader()
{
	loadShader("res/shaders/terrain");
}


TerrainShader::TerrainShader(const std::string& filepath)
{
	loadShader(filepath);
}


TerrainShader::~TerrainShader()
{

}

