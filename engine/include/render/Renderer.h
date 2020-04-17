#pragma once 
#include "render/Window.h"
#include "render/Camera.h"
#include "entity/Scene.h"
#include "render/shaders/ComputeShader.h"
#include "render/shaders/SkyBoxShader.h"
namespace juno {

class Renderer 
{
public:
    static Renderer& get()
    {	
        static Renderer s_instance(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,20.0f,0));
        return s_instance;
    }

    Renderer(const Renderer& renderer) = delete;

    Window& getWindow();
    Camera& getCamera();
    Scene& getScene();

    void submit(Scene& scene);
    void submit(Entity& entity);
    void submit(Entity& entity, std::shared_ptr<Shader> shader);
    void submit(Light& light);

    void render(float dt);
    void updateWindow(float dt);

    void runComputeShader(ComputeShader& cs, float dt);

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