#include "jnpch.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCoreGL.h"
#include "Core/AssetManager.h"
using namespace Juno;


Renderer::Renderer(float sWidth, float sHeight, const std::string& wTitle, glm::vec3 camPos, glm::vec3 camRot) : window(sWidth, sHeight, wTitle), camera(camPos, camRot)
{
    defaultShader = &AssetManager::Get().GetDefaultShader();                      
    window.GetKeyDispatcher().AddListener(&camera);
	window.GetMouseDispatcher().AddListener(&camera);
	window.GetWinEventDispatcher().AddListener(&camera);

}

Window& Renderer::GetWindow()
{
    return window;
}

Camera& Renderer::GetCamera()
{
    return camera;
}

Scene& Renderer::GetScene()
{
    return *scene;
}

void Renderer::Submit(Scene& scene)
{
    this->scene = &scene;
}

void Renderer::Submit(Entity& entity)
{
}

void Renderer::Submit(Entity& entity, std::shared_ptr<Shader> shader)
{
}

void Renderer::Submit(Light &light)
{
}

void Renderer::Render(float dt)
{
    camera.Update(dt);
    RenderCall::Clear();


    std::vector<Entity>& entities = scene->GetEntities();
    //std::vector<std::shared_ptr<Shader>> entity_shaders = queue.getEntityShaders();
    std::vector<Light> lights = scene->GetLights();



    for(unsigned int i = 0; i < scene->GetEntities().size(); i++)
    {   
        defaultShader->Bind();
        defaultShader->LoadProjectionMatrix(camera.GetProjectionMatrix());			
        defaultShader->LoadViewMatrix(camera);
        defaultShader->LoadLightUniforms(lights);
        
        RenderCall::RenderEntity(entities[i] , *defaultShader);
        defaultShader->Unbind();
    }

    /* render skybox */
    // skybox_shader->setActive();
    // skybox_shader->loadProjectionMatrix(camera.getProjectionMatrix());
    // skybox_shader->loadViewMatrix(&camera);
    // glRender::renderSkyBox(scene->getSkyBox(), skybox_shader);


}

void Renderer::UpdateWindow(float dt)
{
    window.Update(*scene, dt);

}


void Renderer::RunComputeShader(ComputeShader& cs, float dt)
{   
    float forceRadii = 10;

    cs.BindCS();
    cs.LoadFloat(glGetUniformLocation(cs.GetCSID(), "timestep"), dt);
    cs.LoadFloat(glGetUniformLocation(cs.GetCSID(), "forceRadii"), forceRadii);

    cs.Compute();

    cs.LoadProjectionMatrix(camera.GetProjectionMatrix());
    cs.LoadViewMatrix(camera);
    RenderCall::ApplyComputeShader(cs);
}