#include "jnpch.h"
#include "Renderer/RenderObjects/Material.h"
#include "Core/AssetManager.h"
#include "Renderer/Renderer.h"
using namespace Juno;

Material::Material()
{
	std::array<ShaderComponentType, 3> shaderComponents = {ShaderComponentType::Vertex, ShaderComponentType::Fragment, ShaderComponentType::Empty };
    shader = AssetManager::Get().LoadShader(shaderComponents, "./Resources/Shaders/basic");
	
    baseColor = glm::vec3(0.5,0.5,0.5);
    reflectivity = 0.4f;
    shineDamper = 0.8f;
    CacheUniforms();
}

Material::~Material()
{

}

SPtr<Shader> Material::GetShader()
{
    return shader;
}

void Material::CacheUniforms()
{
    //shader->LoadFloat()
    loc_reflectivity = shader->GetUniformLocation("reflectivity");
    loc_shineDamper = shader->GetUniformLocation("shineDamper");
    loc_baseColor = shader->GetUniformLocation("baseColor");


	loc_modelMatrix = shader->GetUniformLocation("model");
	loc_viewMatrix = shader->GetUniformLocation("view");
	loc_projectionMatrix = shader->GetUniformLocation("projection");


	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		std::string location("lightPosition["+std::to_string(i)+"]");
		loc_lightPositions[i] = shader->GetUniformLocation(location); 
		location = std::string("lightColor["+std::to_string(i)+"]");
		loc_lightColors[i] = shader->GetUniformLocation(location);
		location = std::string("attenuations["+std::to_string(i)+"]");
		loc_attenuations[i] = shader->GetUniformLocation(location);
	}
}

void Material::Bind()
{
    shader->Bind();

    shader->LoadFloat(loc_reflectivity, reflectivity);
    shader->LoadFloat(loc_shineDamper, shineDamper);
    shader->LoadFloat3(loc_baseColor, baseColor);
}
void Material::Unbind()
{
    shader->Unbind();
}


void Material::SetBaseColor(const glm::vec3& baseColor)
{
    this->baseColor = baseColor;
}

void Material::SetReflectivity(float reflectivity)
{
    this->reflectivity = reflectivity;
}

void Material::SetShineDamper(float shineDamper)
{
    this->shineDamper = shineDamper;
}

void Material::LoadModelMatrix(const Mat4& modelMat)
{
    shader->LoadMat4(loc_modelMatrix, modelMat);
}

void Material::LoadProjectionMatrix(const Mat4& projectionMat)
{
    shader->LoadMat4(loc_projectionMatrix, projectionMat);
}

void Material::LoadViewMatrix(const Mat4& viewMat)
{
    shader->LoadMat4(loc_viewMatrix, viewMat);
}

void Material::LoadLights(std::vector<SPtr<Light>>& lights)
{
	for(unsigned int i = 0; i < NUM_LIGHTS; i++)
	{
		if(i < lights.size())
		{
			shader->LoadFloat3(loc_lightPositions[i], lights[i]->GetPosition());
			shader->LoadFloat3(loc_lightColors[i], lights[i]->GetColor());
			shader->LoadFloat3(loc_attenuations[i], lights[i]->GetAttenuation());
		} else {
			glm::vec3 empty_light(0.0, 0.0, 0.0);
			shader->LoadFloat3(loc_lightPositions[i], empty_light);
			shader->LoadFloat3(loc_lightColors[i], empty_light);
			shader->LoadFloat3(loc_attenuations[i], empty_light);
		}
    }
}

void Material::LoadVarsToShader()
{ 

}
