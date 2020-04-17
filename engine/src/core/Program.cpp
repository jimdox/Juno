#include "pch.h"
#include "core/Program.h"
#include "core/Log.h"
#include "render/glRenderCore.h"

#define MIN_FRAME_TIME 1.0f/240.0f
using namespace juno;

Program::Program() : scene(SceneManager::get().getScene())
{
	this->f_program_should_close = false;
	frame_time = 0;
	delta_time = 0;
}	

Program::~Program()
{

}

void Program::init()
{
	assetManager = &AssetManager::get();
	renderer = &Renderer::get();

	renderer->getWindow().getWinEventDispatcher().addListener(this);
	renderer->getWindow().getGuiDock().addListener(this);
	renderer->getCamera().addListener(this);
	onCreate();

}

void Program::onAttach()
{
	
}

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
	case EventType::SCENE_SWITCH:
		
		if(((const CustomWindowEvent&) e).sceneCode == 0)
		{
			runExampleGeneral();
		} else if(((const CustomWindowEvent&)e).sceneCode == 1)
		{
			runExampleParticleSim();
		} 
		
		break;
	default:
		break;
	}
}

void Program::run()
{
	delta_time = 0;
	frame_time = 0;
	last_time = glfwGetTime();

	while (!programShouldClose())
	{
		onUpdate();

		if(frame_time > MIN_FRAME_TIME)
		{
			renderer->render(frame_time);
			onFrameBufferUpdate();
			renderer->updateWindow(frame_time);
			frame_time = 0;
		}

		delta_time = (glfwGetTime() - last_time);
		last_time = glfwGetTime();
		frame_time += delta_time;
	}
}

bool Program::programShouldClose()
{
	return f_program_should_close || !renderer->getWindow().isRunning();
}

void Program::exit()
{

}









/* placeholders for proper scene and context switching system */
void Program::runExampleParticleSim()
{
	SceneManager::get().getScene().clear();
	delta_time = 0;
	frame_time = 0;
	last_time = glfwGetTime();
	
	std::vector<Particle> particles;
	
	for(unsigned int i = 0; i < 10000; i++)
    {
        glm::vec3 p (100 * cosf(i), 100*sin(i)*cosf(i), 100 * sinf(i * M_PI/300));
        glm::vec3 v (sinf(2 * M_PI * i) + cosf(-i), cosf(i/(2 * M_PI)), sinf(2 * M_PI / i));   
        particles.emplace_back(p, v, 1.0f);
    }
	
	ComputeShader cs(particles, "./sandbox/shaders/example", 512);

	while (!programShouldClose())
	{

		if(frame_time > MIN_FRAME_TIME)
		{
			renderer->render(frame_time);
			renderer->runComputeShader(cs, frame_time);
			renderer->updateWindow(frame_time);
			frame_time = 0;
		}

		delta_time = (glfwGetTime() - last_time);
		last_time = glfwGetTime();
		frame_time += delta_time;
	}
}

void Program::runExamplePhysics()
{

}

void Program::runExampleGeneral()
{
	SceneManager::get().getScene().clear();
	delta_time = 0;
	frame_time = 0;
	last_time = glfwGetTime();

	Mesh plane = AssetManager::get().loadMesh("./engine/res/plane.obj");
	Mesh susy = AssetManager::get().loadMesh("./engine/res/susanne.obj");
	Mesh dragon = AssetManager::get().loadMesh("./engine/res/dragon.obj");

	plane.getMaterial().setBaseColor(glm::vec3(0.1f, 0.1f, 0.1f));
	plane.getMaterial().setReflectivity(0.12);
	dragon.getMaterial().setBaseColor(glm::vec3(0.4f, 0.1f, 0.1f));
	dragon.getMaterial().setShineDamper(0.99f);
	dragon.getMaterial().setReflectivity(0.25f);
	
	Entity entityOne (plane, glm::vec3(0.0f, -5.5f, 0.0f), glm::vec3(0,0, 0), 100.0f, "default plane");
	Entity entityTwo (susy,  glm::vec3(-30.0f, 1.0f, -5.0f), glm::vec3(-50,40,0), 5.0f, "susanne 0");
	Entity entityThree(susy, glm::vec3(-50, 1.0f, 5.0f), glm::vec3(-9, 90, 0), 6.0f, "susanne 1");
	Entity entityFour(susy, glm::vec3(40, 1.0f, 5.0f), glm::vec3(12, 30, 20), 6.5f, "susanne 2");

	Entity entityDragon (dragon, glm::vec3(60, -5, -30), glm::vec3(0,-50,0), 5.0f, "Big Boi");
	
	Light light(glm::vec3(-110.5f, 500.01f, 100.0f), glm::vec3(0.9f, 0.9f, 0.9f), glm::vec3(1.0, 1, 0.005));
	Light lightB(glm::vec3(300,100,-1010), glm::vec3(0.4f, 0.4f, 0.70f), glm::vec3(1.0, 0.00001f, 0.0005f));

	scene.add(entityOne);
	scene.add(entityTwo);
	scene.add(entityThree);
	scene.add(entityFour);
	scene.add(entityDragon);


	scene.add(light);
	scene.add(lightB);

	while (!programShouldClose())
	{

		if(frame_time > MIN_FRAME_TIME)
		{
			renderer->render(frame_time);

			renderer->updateWindow(frame_time);
			frame_time = 0;
		}

		delta_time = (glfwGetTime() - last_time);
		last_time = glfwGetTime();
		frame_time += delta_time;
		exit();
	}
}


