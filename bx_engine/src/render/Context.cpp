#include "render/Context.h"

#define STB_IMAGE_IMPLEMENTATION
#include "render/Texture.h"
#include "render/glRenderCore.h"
#include "core/EngineConfig.h"
#include "core/bxMath.h"
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
	glfwWindowHint(GLFW_SAMPLES, anti_aliasing_factor);								

	glfwSetErrorCallback(setErrCallback);
	glfwSetKeyCallback(window, kbdLayout);
	glfwSetCursorPosCallback(window, mousePositionHandler);
	glfwSetMouseButtonCallback(window, mouseButtonMap);
	glfwSetScrollCallback(window, mouseScrollHandler);

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(window, cursor);
	
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

void Context::updateCamera(Camera* camera, float dt)
{
	glm::vec3 vel(0,0,0);
	glm::vec3 rot(0,0,0);
	float move_speed = camera->DEFAULT_MOVE_SPEED;
	float rot_speed = camera->DEFAULT_ROT_SPEED;
	if(key_pressed_W)
	{
		vel.z = -move_speed*dt;
	} else if(key_pressed_S) {
		vel.z = move_speed*dt;
	}
	if(key_pressed_A)
	{
		vel.x = -move_speed*dt;
	} else if(key_pressed_D) {
		vel.x = move_speed*dt;
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

	if(key_pressed_X)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	// s_cam_vx = 0;
	// s_cam_vy = 0;
	// s_cam_vz = 0;
	
	//BX_INFO("dt: {}", dt);


	camera->update(vel, rot, 0);


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