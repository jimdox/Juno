#include "pch.h"
#include "core/Program.h"
#include "core/Log.h"

#define MIN_FRAME_TIME 1.0f/120.0f
using namespace juno;

Program::Program()
{
	this->f_program_should_close = false;
}	

Program::~Program()
{

}

void Program::init()
{
	onCreate();
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
	delta_time = 0;
	frame_time = 0;
	last_time = glfwGetTime();

	while (!programShouldClose())
	{
		onUpdate();

		if(delta_time > MIN_FRAME_TIME)
		{
			onFrameBufferUpdate();
			renderer->update(delta_time);
			delta_time = 0;
		}

		frame_time = (glfwGetTime() - last_time);
		last_time = glfwGetTime();
		delta_time += frame_time;
	}
}

bool Program::programShouldClose()
{
	return f_program_should_close || !renderer->getContext().isRunning();
}

void Program::exit()
{
	//render_context->~Context();
}
