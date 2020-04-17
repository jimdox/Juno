#include "jnpch.h"
#include "Renderer/Material.h"
#include "Core/AssetManager.h"
#include "Renderer/Renderer.h"
using namespace Juno;

Material::Material()
{
    shaderID = 0;
    baseColor = glm::vec3(0.5,0.5,0.5);
    reflectivity = 0.4f;
    shineDamper = 0.8f;
}

// Material::Material(Shader& shader)
// {
//     shaderID = 0;
//     baseColor = glm::vec3(0.5,0.5,0.5);
//     reflectivity = 0.4f;
//     shineDamper = 0.2f;
// }

Material::~Material()
{

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

