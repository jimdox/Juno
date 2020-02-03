#include "render/Context.h"
#define STB_IMAGE_IMPLEMENTATION
#include "render/textures/Texture.h"
#include "render/glRenderCore.h"
#include "core/EngineConfig.h"
#include "core/MathUtils.h"

/*
	(OpenGL) Rendering context
*/

using namespace juno;

static Context* s_context;

/* /// GLFW Callbacks /// */

	static void mousePositionHandler(GLFWwindow* window, double x_pos, double y_pos)
	{
		s_context->dispatchEvent(MouseMoveEvent(x_pos, y_pos));
	}

	static void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
	{
		switch(action)
		{
		case GLFW_PRESS:
			s_context->dispatchEvent(MousePressEvent(static_cast<MouseCode>(((short) button))));
			break;
		case GLFW_RELEASE:
			s_context->dispatchEvent(MouseReleaseEvent(static_cast<MouseCode>(((short) button))));
			break;
		}
	}
	
	void mouseScrollHandler(GLFWwindow* window, double x_offset, double y_offset)
	{
		s_context->dispatchEvent(MouseScrollEvent(x_offset, y_offset));
	}


Context::Context()
{
	this->width = 400;
	this->height = 400;
	this->title = " ";
	this->isLoading = true;
	this->isValid = true;
	s_context = this;

}

Context::Context(int width, int height, const std::string name)
{
	this->width = width;
	this->height = height;
	this->title = name;
	this->isLoading = true;
	this->isValid = true;
	init();
}

/* set window border T/F */
Context::Context(int width, int height, std::string title, bool border)
{
	this->width = width;
	this->height = height;
	this->title = title;
	this->isLoading = true;
	this->isValid = true;
	if (!border)
	{
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	}
	init();


}


Context::~Context()
{
	destroy();
}
/* closes current glfw window */
void Context::destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwDestroyWindow(window);

}


static void setErrCallback(int code, const char* message)
{
	std::string error(message);
	JN_ERR("GLFW window error code: " + code);
	JN_CRIT(error);
}


void Context::init()
{
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);										// Oldest allowable version of GLSL (4.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);	

	glfwInit();

	if (!glfwInit())	
	{
		JN_ERR("GLFW failed to initialize!");
	}


	this->window = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);	// third argument makes window full screen if:

	if (!window)
	{
		JN_ERR("Window creation failed!");
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);																	/* limits fps to native refresh rate */
	glfwWindowHint(GLFW_SAMPLES, anti_aliasing_factor);								

	glfwSetErrorCallback(setErrCallback);
	// glfwSetKeyCallback(window, kbdLayout);
	// glfwSetCursorPosCallback(window, mousePositionHandler);
	// glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(window, cursor);
	glRender::init(window);
	guiDock.init();

	aspectRatio = this->width / this->height;

}

void Context::update(float dt)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	guiDock.update(dt);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Context::updateCamera(Camera* camera, float dt)
{
	glm::vec3 dPos(0,0,0);
	glm::vec3 rot(0,0,0);
	float move_speed = camera->DEFAULT_MOVE_SPEED;
	float rot_speed = camera->DEFAULT_ROT_SPEED;
	if(key_pressed_W)
	{
		dPos.z = -move_speed*dt;
	} else if(key_pressed_S) {
		dPos.z = move_speed*dt;
	}
	if(key_pressed_A)
	{
		dPos.x = -move_speed*dt;
	} else if(key_pressed_D) {
		dPos.x = move_speed*dt;
	}
	if(key_pressed_LEFT)
	{
		rot.z = -rot_speed*dt;
	} else if(key_pressed_RIGHT)
	{
		rot.z = rot_speed*dt;
	}
	if(key_pressed_UP)
	{
		rot.y = -rot_speed*dt;
	} else if(key_pressed_DOWN)
	{
		rot.y = rot_speed*dt;
	}
	if(key_pressed_R)
	{
		dPos.y = move_speed * dt;
	} else if(key_pressed_F)
	{
		dPos.y = -move_speed * dt;
	}

	if(key_pressed_X)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
		//camera->update(dPos, rot, 0, mouse_data);


}

/*   ---   */


/*
bool RenderContext::isVisible()
{
	return true;
}
*/

void Context::attachListener(EventListener* listener)
{
	addListener(listener);
}

void Context::detatchListener(EventListener* listener)
{
	rmListener(listener);
}

void Context::dispatchEvent(const Event& e)
{
	notify(e);
}

bool Context::isRunning()
{
	return !glfwWindowShouldClose(this->window);
}

int Context::getWidth()
{
	return width;
}

int Context::getHeight()
{
	return height;
}

GLFWwindow* Context::getWindow()
{
	return this->window;
}