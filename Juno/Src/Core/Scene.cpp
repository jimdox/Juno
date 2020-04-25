#include "jnpch.h"
#include "Core/Scene.h"
#include "Core/AssetManager.h"
#include "Utils/Math.h"
#include "Renderer/RenderObjects/ThirdPersonCamera.h"
#include <array>
using namespace Juno;

Scene::Scene() 
{
    camera = std::make_unique<ThirdPersonCamera>(Vec3(0,5,5), Vec3(0,0,0));
}

Scene::Scene(SPtr<Camera> camera)
{
    this->camera = camera;
}

Scene::~Scene()
{
    // for(unsigned int i = 0; i < entities.size(); i++)
    // {
    //     delete entities[i];
    // }
    // for(unsigned int i = 0; i < lights.size(); i++)
    // {
    //     delete lights[i];
    // }
}

void Scene::Add(SPtr<Entity> entity)
{
    entities.push_back(entity);
}

void Scene::Add(SPtr<Light> light)
{
    lights.push_back(light);
}

void Scene::CreateEntity(SPtr<Mesh> mesh, const Vec3& pos, const Vec3& rot, float scale, const std::string& name)
{
    entities.emplace_back(std::make_shared<Entity>(mesh, pos, rot, scale, name));
}

void Scene::CreateLight(const Vec3& pos, const Vec3& color)
{
    lights.emplace_back(std::make_shared<Light>(pos, color));
}


void Scene::ClearScene()
{
    entities.clear();
    lights.clear();
}

std::vector<SPtr<Entity>>& Scene::GetEntities()
{
    return entities;
}

std::vector<SPtr<Light>>& Scene::GetLights()
{
    return lights;
}

SPtr<Camera> Scene::GetCamera()
{
    return camera;
}

// SkyBox& Scene::getSkyBox()
// {
//     return skybox;
// }
