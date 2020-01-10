#pragma once
#include "pch.h"
#include "entity/Entity.h"
#include "Camera.h"
#include "render/shaders/Shader.h"
#include "render/shaders/TerrainShader.h"
#include "entity/terrain/Terrain.h"
#include "render/shaders/SkyBoxShader.h"
#include "entity/SkyBox.h"


namespace juno {
class RenderQueue
{
public:
    RenderQueue();  
    ~RenderQueue();

    void setLights(std::vector<Light> &lights);
    void addLight(Light &light);

    void submit(Entity* entity, std::shared_ptr<Shader> shader);
    void submit(Terrain* terrain, std::shared_ptr<TerrainShader> shader);
    void submit(SkyBox* skybox, std::shared_ptr<SkyBoxShader> shader);

    void remove(Entity& entity);
    void render(Camera* camera);
    void clear();

private:
    std::vector<Entity *> entities;
    std::vector<std::shared_ptr<Shader>> entity_shaders;
    std::vector<Entity *> instanced_entities;
    std::vector<Light> lights;

    std::vector<Terrain *> terrainBlocks;
    std::vector<std::shared_ptr<TerrainShader>> terrain_shaders;

    SkyBox* skybox;
    std::shared_ptr<SkyBoxShader> skyboxShader;

};
}