#include "render/Renderer.h"

using namespace juno;



Renderer::Renderer(float sc_width, float sc_height, const std::string& w_title, glm::vec3 cam_pos, glm::vec3 cam_rot) : window(sc_width, sc_height, w_title), camera(cam_pos, cam_rot)
{
	// JN_WARN(JN_GFX_DEVICE);
    default_shader = std::make_shared<Shader>("./juno_engine/res/shaders/basic");												        /* useful for debugging w/ hybrid graphics */
	//skybox_shader = std::make_shared<SkyBoxShader>("./juno_engine/res/shaders/skybox");
    window.getKeyDispatcher().addListener(&camera);
	window.getMouseDispatcher().addListener(&camera);
	window.getWinEventDispatcher().addListener(&camera);
	default_shader->setActive();
	default_shader->loadProjectionMatrix(camera.getProjectionMatrix());				/* load the perspective, view matrix from Camera */
	default_shader->loadViewMatrix(&camera);
	default_shader->unbindProgram();
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

void Renderer::submit(Scene* scene)
{
    this->scene = scene;
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

void Renderer::update(float delta_time)
{
    camera.update();
    glRender::clear();

    std::vector<Entity>& entities = scene->getEntities();
    //std::vector<std::shared_ptr<Shader>> entity_shaders = queue.getEntityShaders();
    std::vector<Light> lights = scene->getLights();



    for(unsigned int i = 0; i < scene->getEntities().size(); i++)
    {   
        default_shader->setActive();
        default_shader->loadProjectionMatrix(camera.getProjectionMatrix());			
        default_shader->loadViewMatrix(&camera);
        default_shader->loadLightUniforms(lights);
        
        glRender::renderEntity(entities[i] , default_shader);
        
        default_shader->unbindProgram();
    }

    /* render skybox */
    // skybox_shader->setActive();
    // skybox_shader->loadProjectionMatrix(camera.getProjectionMatrix());
    // skybox_shader->loadViewMatrix(&camera);
    // glRender::renderSkyBox(scene->getSkyBox(), skybox_shader);

    window.update(*scene, delta_time);
}