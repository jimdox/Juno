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

static KeyEventDispatcher* s_keyDispatcher;
static MouseEventDispatcher* s_mouseDispatcher;
#define DEFAULT_SCREEN_WIDTH = 1080
#define DEFAULT_SCREEN_HEIGHT = 1920
/* /// GLFW Callbacks /// */

static inline void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{	 
	if(action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		s_keyDispatcher->notify(KeyPressEvent(key, 0));						/* todo: key repeat counting */
	} else {
		s_keyDispatcher->notify(KeyReleaseEvent(key));
	}
}

static inline void mousePositionHandler(GLFWwindow* window, double x_pos, double y_pos)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	JN_INFO(x_pos - width/2);
	JN_INFO(y_pos - height/2);
	s_mouseDispatcher->notify(MouseMoveEvent(x_pos - width/2, y_pos - height/2));
	//JN_CRIT("Mouse is moving!");
}

static inline void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		switch(button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			s_mouseDispatcher->notify(MousePressEvent(MouseCode::M_BUTTON_LEFT));
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			s_mouseDispatcher->notify(MousePressEvent(MouseCode::M_BUTTON_MID));
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			s_mouseDispatcher->notify(MousePressEvent(MouseCode::M_BUTTON_RIGHT));
			break;
		default:					/* discard all other mouse codes */
			break;
		}
	} else {					
		switch(button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
				s_mouseDispatcher->notify(MouseReleaseEvent(MouseCode::M_BUTTON_LEFT));
				break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			s_mouseDispatcher->notify(MouseReleaseEvent(MouseCode::M_BUTTON_MID));
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			s_mouseDispatcher->notify(MouseReleaseEvent(MouseCode::M_BUTTON_RIGHT));
			break;
		default:					/* discard all other mouse codes */
			break;
		}
	}
}

void inline mouseScrollHandler(GLFWwindow* window, double x_offset, double y_offset)
{
	s_mouseDispatcher->notify(MouseScrollEvent(x_offset, y_offset));
}
/* /// End Callbacks /// */


Context::Context()
{
	this->width = 400;
	this->height = 400;
	this->title = " ";


}

Context::Context(int width, int height, const std::string name)
{
	this->width = width;
	this->height = height;
	this->title = name;
	init();
}

/* set window border T/F */
Context::Context(int width, int height, std::string title, bool border)
{
	this->width = width;
	this->height = height;	
	this->title = title;

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

	//glfwSwapInterval(1);																	/* limits fps to native refresh rate */
	glfwWindowHint(GLFW_SAMPLES, anti_aliasing_factor);				

	s_keyDispatcher = &this->keyDispatcher;
	s_mouseDispatcher = &this->mouseDispatcher;				

	glfwSetErrorCallback(setErrCallback);
	glfwSetKeyCallback(window, keyboardHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetCursorPosCallback(window, mousePositionHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);

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

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

/*   ---   */


/*
bool RenderContext::isVisible()
{
	return true;
}
*/

KeyEventDispatcher& Context::getKeyDispatcher()
{
	return *s_keyDispatcher;
}

MouseEventDispatcher& Context::getMouseDispatcher()
{
	return *s_mouseDispatcher;
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

