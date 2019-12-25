#include "render/Context.h"
#define STB_IMAGE_IMPLEMENTATION
#include "render/textures/Texture.h"
#include "render/glRenderCore.h"
#include "core/EngineConfig.h"
#include "core/bxMath.h"

/*
	(OpenGL) Rendering context
*/

using namespace bx;

void mousePositionHandler(GLFWwindow* window, double x_pos, double y_pos)
{
    if(!MOUSE_DATA_RECIEVED)
    {
        CURSOR_DX += x_pos - CENTER_X;
        CURSOR_DY += y_pos - CENTER_Y;
    } else {
        CURSOR_DX = x_pos - (CENTER_X);
        CURSOR_DY = y_pos - (CENTER_Y);
        MOUSE_DATA_RECIEVED = false;
    }
    CURSOR_X += CURSOR_DX;
    CURSOR_Y += CURSOR_DY;
}

void mouseButtonMap(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			LM_BUTTON_PRESS = true;
			LM_BUTTON_REPEAT = true;
		} else {
			LM_BUTTON_PRESS = false;
			LM_BUTTON_REPEAT = false;
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			RM_BUTTON_PRESS = true;
		} else {
			RM_BUTTON_PRESS = false;
		}
	}
}

void mouseScrollHandler(GLFWwindow* window, double x_offset, double y_offset)
{
    if(!MOUSE_DATA_RECIEVED)
    {
        SCROLL_DX += SCROLL_X - x_offset;
        SCROLL_DY += SCROLL_Y - y_offset;
    } else {
        SCROLL_DX = SCROLL_X - x_offset;
        SCROLL_DY = SCROLL_Y - y_offset;
        MOUSE_DATA_RECIEVED = false;
    }
    SCROLL_X += x_offset;
    SCROLL_Y += y_offset;
}


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
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwDestroyWindow(window);

}

/* /// GLFW Callbacks /// */



static void setErrCallback(int code, const char* message)
{
	std::string error(message);
	BX_ERR("GLFW window error code: " + code);
	BX_CRIT(error);
}

/* /// class methods /// */

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
	guiDock.init();

	aspectRatio = this->width / this->height;

	CENTER_X = width/2.0;
	CENTER_Y = height/2.0;

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
		mouse_data.LM_BUTTON_PRESS = LM_BUTTON_PRESS;
		mouse_data.RM_BUTTON_PRESS = RM_BUTTON_PRESS;
		mouse_data.CURSOR_X = CURSOR_X;
		mouse_data.CURSOR_Y = CURSOR_Y;
		mouse_data.CURSOR_DX = CURSOR_DX;
		mouse_data.CURSOR_DY = CURSOR_DY;
		mouse_data.SCROLL_X = SCROLL_X;
		mouse_data.SCROLL_Y = SCROLL_Y;
		mouse_data.SCROLL_DX = SCROLL_DX;
		mouse_data.SCROLL_DY = SCROLL_DY;
		MOUSE_DATA_RECIEVED = true;

		camera->update(dPos, rot, 0, mouse_data);


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