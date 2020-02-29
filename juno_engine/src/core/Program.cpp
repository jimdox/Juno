#include "pch.h"
#include "core/Program.h"
#include "core/Log.h"

#define DISPLAY_MAX_FPS 120
using namespace juno;

Program* s_prog_instance;

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
	onCreate();
	renderer->getContext().getWinEventDispatcher().addListener(this);
	renderer->getCamera().addListener(this);
	s_prog_instance = this;

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
	/* ----- */
	last_time = glfwGetTime();

	while (!programShouldClose())
	{
		onUpdate();

		if(frame_time > 1/(DISPLAY_MAX_FPS))
		{
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
	return f_program_should_close || !renderer->getContext().isRunning();
}

void Program::exit()
{
	//render_context->~Context();
}
