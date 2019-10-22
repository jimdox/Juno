#include "render/RenderQueue.h"
#include "render/glRenderCore.h"
using namespace bx;

RenderQueue::RenderQueue(std::shared_ptr<Camera> camera) : camera(camera)
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

void RenderQueue::remove(Entity& entity)
{
    for(int i = 0; i < entities.size(); i++)
    {
        
    }
}

void RenderQueue::render()
{
    for(int i = 0; i < entities.size(); i++)
    {   
        shaders[i]->useProgram();
        shaders[i]->loadViewMatrix(camera);
        shaders[i]->loadLightUniforms(lights[0]);
        bxRender::renderEntity(entities[i] , shaders[i]);
        shaders[i]->unbindProgram();
    }
}

void RenderQueue::clear()
{
    entities.clear();
}



