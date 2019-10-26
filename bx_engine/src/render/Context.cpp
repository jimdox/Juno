#include "render/Context.h"

#define STB_IMAGE_IMPLEMENTATION
#include "render/Texture.h"
#include "render/glRenderCore.h"
#include "core/EngineConfig.h"
/*
	(OpenGL) Rendering context
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
	std::string error(message);
	BX_ERR("GLFW window error code: " + code);
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

void Context::update(Camera* camera, float dt)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glfwSwapBuffers(window);
	glfwPollEvents();

	float vx = 0;
	float vy = 0;
	float vz = 0;

	if(key_pressed_W)
	{
		vz = -MOVE_SPEED * dt;
	}
	else if(key_pressed_S)
	{
		vz = MOVE_SPEED * dt;
	}
	if(key_pressed_X && cam_mode_xray)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		cam_mode_xray = false;
	} else if(key_pressed_X)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		cam_mode_xray = true;
	}
	key_pressed_X = false;
	key_pressed_W = false;
	key_pressed_S = false;
	// s_cam_vx = 0;
	// s_cam_vy = 0;
	// s_cam_vz = 0;
	


	camera->update(glm::vec3(s_cam_vx*dt, s_cam_vy*dt, vz), glm::vec3(s_cam_roll, s_cam_pitch, s_cam_yaw), s_deltaZoom);


	key_pressed_W = false;
	key_pressed_S = false;

}

/*   ---   */


/*
bool RenderContext::isVisible()
{
	return true;
}
*/

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