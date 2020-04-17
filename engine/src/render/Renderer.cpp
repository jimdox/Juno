#include "pch.h"
#include "render/Renderer.h"
#include "render/glRenderCore.h"
#include "core/AssetManager.h"
using namespace juno;



Renderer::Renderer(float sc_width, float sc_height, const std::string& w_title, glm::vec3 cam_pos, glm::vec3 cam_rot) : window(sc_width, sc_height, w_title), camera(cam_pos, cam_rot)
{
    defaultShader = &AssetManager::get().getDefaultShader();                      
    window.getKeyDispatcher().addListener(&camera);
	window.getMouseDispatcher().addListener(&camera);
	window.getWinEventDispatcher().addListener(&camera);

}

Window& Renderer::getWindow()
{
    return window;
}

Camera& Renderer::getCamera()
{
    return camera;
}

Scene& Renderer::getScene()
{
    return *scene;
}

void Renderer::submit(Scene& scene)
{
    this->scene = &scene;
}

void Renderer::submit(Entity& entity)
{
}

void Renderer::submit(Entity& entity, std::shared_ptr<Shader> shader)
{
}

void Renderer::submit(Light &light)
{
}

void Renderer::render(float dt)
{
    camera.update(dt);
    glRender::clear();


    std::vector<Entity>& entities = scene->getEntities();
    //std::vector<std::shared_ptr<Shader>> entity_shaders = queue.getEntityShaders();
    std::vector<Light> lights = scene->getLights();



    for(unsigned int i = 0; i < scene->getEntities().size(); i++)
    {   
        defaultShader->setActive();
        defaultShader->loadProjectionMatrix(camera.getProjectionMatrix());			
        defaultShader->loadViewMatrix(&camera);
        defaultShader->loadLightUniforms(lights);
        
        glRender::renderEntity(entities[i] , *defaultShader);
        defaultShader->unbindProgram();
    }

    /* render skybox */
    // skybox_shader->setActive();
    // skybox_shader->loadProjectionMatrix(camera.getProjectionMatrix());
    // skybox_shader->loadViewMatrix(&camera);
    // glRender::renderSkyBox(scene->getSkyBox(), skybox_shader);


}

void Renderer::updateWindow(float dt)
{
    window.update(*scene, dt);

}


void Renderer::runComputeShader(ComputeShader& cs, float dt)
{   
    float forceRadii = 10;

    cs.bindCS();
    cs.loadFloat(glGetUniformLocation(cs.getCSID(), "timestep"), dt);
    cs.loadFloat(glGetUniformLocation(cs.getCSID(), "forceRadii"), forceRadii);

    cs.run();

    cs.loadProjectionMatrix(camera.getProjectionMatrix());
    cs.loadViewMatrix(&camera);
    glRender::applyComputeShader(cs);
}