#include "render/bxContext.h"
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
using namespace bbx;

bxContext::bxContext()
{
	this->width = 400;
	this->height = 400;
	this->title = " ";
	this->isLoading = true;
	this->isValid = true;

}

bxContext::bxContext(int width, int height, const std::string name)
{
	this->width = width;
	this->height = height;
	this->title = name;
	this->isLoading = true;
	this->isValid = true;
	init();
}

/* set window border T/F */
bxContext::bxContext(int width, int height, std::string title, bool border)
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


bxContext::~bxContext()
{
	destroy();
}
/* closes current glfw window */
void bxContext::destroy()
{
	glfwDestroyWindow(window);

}

static void setErrCallback(int code, const char* message)
{
	std::string error(message); // convert char* to string
	BBX_ERR("GLFW window error, code: " + code);
	BBX_CRIT(error);
}



void bxContext::init()
{
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);										// Oldest allowable version of GLSL (4.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);	



	glfwInit();

	if (!glfwInit())	
	{
		BBX_ERR("GLFW failed to initialize!");
	}


	this->window = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);	// third argument makes window full screen if:

	if (!window)
	{
		BBX_ERR("Window creation failed!");
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);																	/* limits fps to native refresh rate */


	glfwSetErrorCallback(setErrCallback);
	glfwSetKeyCallback(window, kbdLayout);

	bxRender::init(window);

	//BBX_WARN(glGetString(GL_RENDERER));													/* list video card */
	aspectRatio = this->width / this->height;
}

void bxContext::update()
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
void bxContext::setShader(GLuint id)
{
	this->shaderID = id;
}

bool bxContext::isRunning()
{
	return !glfwWindowShouldClose(this->window);
}

int bxContext::getWidth()
{
	return width;
}

int bxContext::getHeight()
{
	return height;
}

GLFWwindow* bxContext::getWindow()
{
	return this->window;
}
