#include "render/RenderQueue.h"
#include "render/glRenderCore.h"

using namespace juno;

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
    entity_shaders.push_back(shader);
}

void RenderQueue::submit(Terrain* terrain, std::shared_ptr<TerrainShader> ter_shader)
{
    terrainBlocks.push_back(terrain);
    terrain_shaders.push_back(ter_shader);
}

void RenderQueue::submit(SkyBox* skybox, std::shared_ptr<SkyBoxShader> skyboxShader)
{
    this->skybox = skybox;
    this-> skyboxShader = skyboxShader;
}

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
        entity_shaders[i]->useProgram();
	    entity_shaders[i]->loadProjectionMatrix(camera->getProjectionMatrix());				/* load the perspective matrix from Camera */
        entity_shaders[i]->loadViewMatrix(camera);
        entity_shaders[i]->loadLightUniforms(lights);
        glRender::renderEntity(entities[i] , entity_shaders[i]);
        entity_shaders[i]->unbindProgram();
    }

    for(int j = 0; j < terrainBlocks.size(); j++)
    {
        terrain_shaders[j]->useProgram();
        terrain_shaders[j]->loadViewMatrix(camera);
        terrain_shaders[j]->loadLightUniforms(lights);
        glRender::renderTerrain(terrainBlocks[j] , terrain_shaders[j]);
        terrain_shaders[j]->unbindProgram();
    }
    if(skybox != nullptr)
    {
        // skyboxShader->loadProjectionMatrix(camera->getProjectionMatrix());
        // skyboxShader->loadViewMatrix(camera);
        // glRender::renderSkyBox(skybox, skyboxShader);
    }
}

void RenderQueue::clear()
{
    entities.clear();
}



