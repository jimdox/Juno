#include "pch.h"
#include "core/Program.h"
#include "core/Log.h"
#include "render/glRenderCore.h"

#define MIN_FRAME_TIME 1.0f/240.0f
using namespace juno;

Program::Program()
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

	renderer = &Renderer::init(1920, 1080, " ", glm::vec3(0,0,0), glm::vec3(0,20.0f,0));
	renderer->getWindow().getWinEventDispatcher().addListener(this);
	renderer->getWindow().getGuiDock().addListener(this);
	renderer->getCamera().addListener(this);
	onCreate();

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
	delta_time = 0;
	frame_time = 0;
	last_time = glfwGetTime();

	while (!programShouldClose())
	{

		if(frame_time > MIN_FRAME_TIME)
		{
			onUpdate();
			onFrameBufferUpdate();
			renderer->update(frame_time);
			frame_time = 0;
		}

		delta_time = (glfwGetTime() - last_time);
		last_time = glfwGetTime();
		frame_time += delta_time;
	}
}

Scene& Program::getScene()
{
	return scene;
}

bool Program::programShouldClose()
{
	return f_program_should_close || !renderer->getWindow().isRunning();
}

void Program::exit()
{
	//render_context->~Window();
}
