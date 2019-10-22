#include "render/Context.h"
#define STB_IMAGE_IMPLEMENTATION
#include "core/KeyMap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "render/Texture.h"
#include "render/glRenderCore.h"
#include "core/EngineConfig.h"
/*
	(OpenGL/Vulkan) Rendering context
*/
using namespace bx;

Context::Context()
{
	this->width = 400;
	this->height = 400;
	this->title = " ";
	this->isLoading = true;
	this->isValid = true;

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
	glfwDestroyWindow(window);

}

static void setErrCallback(int code, const char* message)
{
	std::string error(message); // convert char* to string
	BX_ERR("GLFW window error, code: " + code);
	BX_CRIT(error);
}



void Context::init()
{
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);										// Oldest allowable version of GLSL (4.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);	



	glfwInit();

	if (!glfwInit())	
	{
		BX_ERR("GLFW failed to initialize!");
	}


	this->window = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);	// third argument makes window full screen if:

	if (!window)
	{
		BX_ERR("Window creation failed!");
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);																	/* limits fps to native refresh rate */


	glfwSetErrorCallback(setErrCallback);
	glfwSetKeyCallback(window, kbdLayout);

	bxRender::init(window);

	//BX_WARN(glGetString(GL_RENDERER));													/* list video card */
	aspectRatio = this->width / this->height;
}

void Context::update()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glfwSwapBuffers(window);
	glfwPollEvents();


}
/*   ---   */


/*
bool RenderContext::isVisible()
{
	return true;
}
*/

/* sets default shader for context */
void Context::setShader(GLuint id)
{
	this->shaderID = id;
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