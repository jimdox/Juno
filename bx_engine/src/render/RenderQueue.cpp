#include "render/RenderQueue.h"
#include "render/glRenderCore.h"
using namespace bx;

RenderQueue::RenderQueue()  //std::shared_ptr<Camera> & camera) : camera(camera)
{

}

RenderQueue::~RenderQueue()
{
    clear();
}

void RenderQueue::setLights(std::vector<Light> &lights)
{
    this->lights = lights;
}

void RenderQueue::addLight(Light &light)
{
    lights.push_back(light);
}

void RenderQueue::submit(Entity* entity, std::shared_ptr<Shader> shader)
{
    entities.push_back(entity);
    shaders.push_back(shader);
}

// void RenderQueue::submit(Terrain* terrain, std::shared_ptr<Shader> shader)
// {
//     terrainBlocks.push_back(terrain);
//     terrainShaders.push_back(shader);
// }

void RenderQueue::remove(Entity& entity)
{
    for(int i = 0; i < entities.size(); i++)
    {
        
    }
}

void RenderQueue::render(Camera* camera)
{


    for(int i = 0; i < entities.size(); i++)
    {   
        shaders[i]->useProgram();
        shaders[i]->loadViewMatrix(camera);
        shaders[i]->loadLightUniforms(lights);
        bxRender::renderEntity(entities[i] , shaders[i]);
        shaders[i]->unbindProgram();
    }

    // for(int j = 0; j < terrainBlocks.size(); j++)
    // {
    //     shaders[j]->useProgram();
    //     shaders[j]->loadViewMatrix(camera);
    //     shaders[j]->loadLightUniforms(lights[0]);
    //     bxRender::renderTerrain(terrainBlocks[j], shaders[j]);
    // }
}

void RenderQueue::clear()
{
    entities.clear();
}



