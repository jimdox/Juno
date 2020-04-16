#include "render/Renderer.h"
#include "render/glRenderCore.h"
#include "core/AssetManager.h"
using namespace juno;



Renderer::Renderer(float sc_width, float sc_height, const std::string& w_title, glm::vec3 cam_pos, glm::vec3 cam_rot) : window(sc_width, sc_height, w_title), camera(cam_pos, cam_rot), computeShader("./sandbox/shaders/comp_test", 1000000, 128)
{
	// JN_WARN(JN_GFX_DEVICE);                                                                          /* useful for debugging w/ hybrid graphics */
    defaultShader = &AssetManager::get().getDefaultShader();                      
    computeShader.setup();
	//skybox_shader = std::make_shared<SkyBoxShader>("./engine/res/shaders/skybox");
    window.getKeyDispatcher().addListener(&camera);
	window.getMouseDispatcher().addListener(&camera);
	window.getWinEventDispatcher().addListener(&camera);
	defaultShader->setActive();
	defaultShader->loadProjectionMatrix(camera.getProjectionMatrix());				                    /* load the perspective, view matrix from Camera */
	defaultShader->loadViewMatrix(&camera);
	defaultShader->unbindProgram();
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
    camera.update(delta_time);
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

    window.update(*scene, delta_time);

}

void Renderer::runComputeShader(float dt)
{   
    camera.update(dt);
    glRender::clear();

    glm::vec3 forceRadii(10.0f, 10.0f, 10.0f);
    

    glUseProgram(computeShader.getCSID());
    // GLuint ssbo = computeShader.getParticleSSBO();
    computeShader.loadFloat(glGetUniformLocation(computeShader.getCSID(), "timestep"), dt);
    computeShader.loadFloat3(glGetUniformLocation(computeShader.getCSID(), "forceRadii"), forceRadii);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, computeShader.getParticleSSBO());
    
    glDispatchCompute( (computeShader.getNumObjects()/128)+1, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    //glMemoryBarrier(GL_ALL_BARRIER_BITS);



    glUseProgram(computeShader.getID());

    glBindVertexArray(computeShader.getParticleVaoID());
    computeShader.loadProjectionMatrix(camera.getProjectionMatrix());
    computeShader.loadViewMatrix(&camera);

    glDrawArraysInstanced(GL_POINTS, 0, 1, computeShader.getNumObjects());

    
    GLuint rendererBuffer = computeShader.getParticleSSBO();
    //glDeleteBuffers(1, &rendererBuffer);

    glBindVertexArray(0);
    glUseProgram(0);
    
    window.update(*scene, dt);


}