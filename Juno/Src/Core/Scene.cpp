#include "jnpch.h"
#include "Core/Scene.h"
#include "Core/AssetManager.h"
#include <array>
using namespace Juno;

Scene::Scene() 
{

}

Scene::~Scene()
{
    Clear();
}

void Scene::Add(Entity& entity)
{
    entities.push_back(entity);
}

void Scene::Add(Light& light)
{
    lights.push_back(light);
}

void Scene::Clear()
{
    entities.clear();
    lights.clear();
}

std::vector<Entity>& Scene::GetEntities()
{
    return entities;
}

std::vector<Light>& Scene::GetLights()
{
    return lights;
}

// SkyBox& Scene::getSkyBox()
// {
//     return skybox;
// }
