#pragma once 
#include "Renderer/Window.h"
#include "Renderer/Camera.h"
#include "Core/Scene.h"
#include "Renderer/Shaders/ComputeShader.h"
//#include "Renderer/Shaders/SkyBoxShader.h"
namespace Juno {

class Renderer 
{
public:
    static Renderer& Get()
    {	
        static Renderer s_instance(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,20.0f,0));
        return s_instance;
    }

    Renderer(const Renderer& renderer) = delete;

    Window& GetWindow();
    Camera& GetCamera();
    Scene& GetScene();

    void Submit(Scene& scene);
    void Submit(Entity& entity);
    void Submit(Entity& entity, std::shared_ptr<Shader> shader);
    void Submit(Light& light);

    void Render(float dt);
    void UpdateWindow(float dt);

    void RunComputeShader(ComputeShader& cs, float dt);

private:
    Renderer(float sc_width, float sc_height, const std::string& window_title, glm::vec3 cam_pos, glm::vec3 cam_rot);

    Window window;
    Scene* scene;
    Camera camera;
    Shader* defaultShader;
    //SkyBoxShader* skyboxShader;

    // static Renderer s_instance;
};
}