#include "render/Renderer.h"

using namespace juno;



Renderer::Renderer(float sc_width, float sc_height, const std::string& w_title, glm::vec3 cam_pos, glm::vec3 cam_rot) : context(sc_width, sc_height, w_title), camera(cam_pos, cam_rot)
{
	// JN_WARN(JN_GFX_DEVICE);
    default_shader = std::make_shared<Shader>("./juno_engine/res/shaders/basic");												        /* useful for debugging w/ hybrid graphics */
	context.getKeyDispatcher().addListener(&camera);
	context.getMouseDispatcher().addListener(&camera);
	context.getWinEventDispatcher().addListener(&camera);
	default_shader->useProgram();
	default_shader->loadProjectionMatrix(camera.getProjectionMatrix());				/* load the perspective, view matrix from Camera */
	default_shader->loadViewMatrix(&camera);
	default_shader->unbindProgram();
}

Context& Renderer::getContext()
{
    return context;
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
        default_shader->useProgram();
	    default_shader->loadProjectionMatrix(camera.getProjectionMatrix());			
        default_shader->loadViewMatrix(&camera);
        default_shader->loadLightUniforms(lights);
        
        glRender::renderEntity(entities[i] , default_shader);
        
        default_shader->unbindProgram();
    }

    context.update(*scene, delta_time);
}