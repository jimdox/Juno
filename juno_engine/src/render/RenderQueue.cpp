#include "render/RenderQueue.h"
#include "render/glRenderCore.h"

using namespace juno;

RenderQueue::RenderQueue()
{

}

RenderQueue::~RenderQueue()
{
    clear();
}

std::vector<Light>& RenderQueue::getLights()
{
    return lights;
}

std::vector<Entity *>& RenderQueue::getEntities()
{
    return entities;
}

std::vector<std::shared_ptr<Shader>>& RenderQueue::getEntityShaders()
{
    return entity_shaders;
}

void RenderQueue::addLights(std::vector<Light>& new_lights)
{
    for(Light light : new_lights)
    {
        this->lights.push_back(light);
    }
}

void RenderQueue::addLight(Light &light)
{
    lights.push_back(light);
}

void RenderQueue::submit(Entity* entity, std::shared_ptr<Shader> shader)
{
    entities.push_back(entity);
    entity_shaders.push_back(shader);
}

void RenderQueue::submit(Terrain* terrain, std::shared_ptr<TerrainShader> ter_shader)
{
    terrainBlocks.push_back(terrain);
    terrain_shaders.push_back(ter_shader);
}

// void RenderQueue::submit(SkyBox* skybox, std::shared_ptr<SkyBoxShader> skyboxShader)
// {
//     this->skybox = skybox;
//     this-> skyboxShader = skyboxShader;
// }

void RenderQueue::remove(Entity& entity)
{
    for(int i = 0; i < entities.size(); i++)
    {
        // if(*entities[i].getID() == entity.getID())
        // {
        //     entities.erase(entities.begin() + i, entities.begin() + (i+1));
        // }
    }
}

void RenderQueue::clear()
{
    entities.clear();
    lights.clear();
}



