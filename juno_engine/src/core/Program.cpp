#include "pch.h"
#include "core/Program.h"
#include "render/glRenderCore.h"
#include "core/Log.h"
#include "core/AssetLoader.h"
#include "render/lights/Light.h"
#include "render/RenderQueue.h"
#include "entity/SkyBox.h"
#include "render/shaders/SkyBoxShader.h"
#include "events/RenderEvent.h"

#define DISPLAY_MAX_FPS 120
using namespace juno;

Program::Program()
{
	renderer = &Renderer::init(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,0,0));
	this->f_program_should_close = false;
	frame_time = 0;
	num_frames = 0;
	delta_time = 0;
	init();
}	

Program::~Program()
{

}

void Program::init()
{
	renderer->getContext().getWinEventDispatcher().addListener(this);
	renderer->getCamera().addListener(this);

}	

void Program::onAttach()
{}

void Program::onEvent(const Event& e)
{
	switch(e.getType())
	{
	case EventType::WINDOW_CLOSE:
		this->f_program_should_close = true;
		break;

	case EventType::WINDOW_RESIZE:
		glRender::resizeFrameBuffer(((const WindowResizeEvent&)e).getScreenWidth(), ((const WindowResizeEvent&)e).getScreenHeight());
		break;
	case EventType::RENDER_POLYGON_WIREFRAME:
		glRender::setModeWireframe(((const RenderWireframeEvent&)e).modeSetWireframe());
		break;
	default:
		break;
	}
}

void Program::run()
{
	Mesh susy = loadModel("./juno_engine/res/asteroid1.obj");
	Texture texture_one("./juno_engine/res/grey.png", TX_DIFFUSE, true);
	susy.addTexture(&texture_one, texture_one.getTexType());

	Mesh plane = loadModel("./juno_engine/res/plane.obj");
	Texture texture_two("./juno_engine/res/green.png", TX_DIFFUSE, true);
	plane.addTexture(&texture_two, texture_two.getTexType());

	glm::vec3 ent_pos(0.0f, 2.8f, -8.0f);
	glm::vec3 ent_rot(0.0f, 12.0f, 0.00f);

	glm::vec3 ground_pos(0.0f, -2.0f, -1.0f);
	glm::vec3 ground_rot(0.0f, 0.0f, 0.0f);

	Entity entity_one(susy, ent_pos, ent_rot, 4.75f, "Entity One");
	Entity entity_two(plane, ground_pos, ground_rot, 70, "Entity Two");
	Material gr_mat;
	gr_mat.reflectivity = 0.001f;
	gr_mat.shineDamper = 0.95f;
	entity_two.getMesh().setMaterial(gr_mat);

	Light light(glm::vec3(110.5f, 100.01f, 100.0f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(1.0, 0.00001, 0.0005));
	Light light_b(glm::vec3(-30.5f, 10.0f, 1.0f), glm::vec3(0.8f, 0.8f, 0.9f), glm::vec3(1.0, 0.0001, 0.0005));

	renderer->submit(entity_one);
	renderer->submit(entity_two);	

	renderer->submit(light);
	renderer->submit(light_b);

	//CubeMap cubemap(cubemapTextures, TX_DIFFUSE);
	// std::shared_ptr skyboxShader = std::make_shared<SkyBoxShader>();
	// SkyBox skybox(cubemap);
	//queue.submit(&skybox, skyboxShader);

	/* ----- */
	last_time = glfwGetTime();

	while (!f_program_should_close && renderer->getContext().isRunning())
	{
		glm::vec3 rotation(0.0f, 0.01f, 0.0f);
		entity_one.addRotation(rotation);

		if(delta_time > 1/(DISPLAY_MAX_FPS))
		{
			renderer->update(delta_time);
			delta_time = 0;
		}

		frame_time = (glfwGetTime() - last_time);
		last_time = glfwGetTime();
		delta_time += frame_time;
	}
}

void Program::exit()
{
	//render_context->~Context();
}
