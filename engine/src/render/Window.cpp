#include "render/Window.h"
#include "render/textures/Texture.h"
#include "render/glRenderCore.h"
#include "core/EngineConfig.h"
//#include "core/AssetManager.h"

#ifdef JN_RENDERER_OPENGL
	#include "glRenderCore.h"
#elseif JN_RENDERER_VULKAN
	#include "vkRenderCore.h"
#endif

/*
	Rendering context: manages the window, initialization of rendering environment. 
*/
using namespace juno;

/* These get passed to glfw callbacks to get IO events */
static KeyEventDispatcher* s_keyDispatcher;
static MouseEventDispatcher* s_mouseDispatcher;
static WindowEventDispatcher* s_winDispatcher;
/* ---- */


/* /// GLFW Callbacks /// */

static void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(!ImGui::GetIO().WantCaptureKeyboard)										/* check if imgui is using keyboard input */
	{
		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			s_keyDispatcher->notify(KeyPressEvent(key, 0));						/* todo: key repeat counting */
		} else {
			s_keyDispatcher->notify(KeyReleaseEvent(key));
		}
	}
}

static void mousePositionHandler(GLFWwindow* window, double x_pos, double y_pos)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	s_mouseDispatcher->notify(MouseMoveEvent(x_pos - width/2, y_pos - height/2));
	//JN_CRIT("Mouse is moving!");
}

static void mouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		switch(button)
		{
		case GLFW_MOUSE_BUTTON_1:
			s_mouseDispatcher->notify(MousePressEvent(MouseCode::M_BUTTON_LEFT));
			break;
		case GLFW_MOUSE_BUTTON_3:
			s_mouseDispatcher->notify(MousePressEvent(MouseCode::M_BUTTON_MID));
			break;
		case GLFW_MOUSE_BUTTON_2:
			s_mouseDispatcher->notify(MousePressEvent(MouseCode::M_BUTTON_RIGHT));
			break;
		default:								/* discard all other mouse codes */
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
		default:								
			break;
		}
	}
}

static void mouseScrollHandler(GLFWwindow* window, double x_offset, double y_offset)
{
	s_mouseDispatcher->notify(MouseScrollEvent(x_offset, y_offset));
}

void windowResizeHandler(GLFWwindow* window, int width, int height)
{
	s_winDispatcher->notify(WindowResizeEvent(width, height));
}	
/* /// End Callbacks /// */

Window::Window()
{
	this->width = 1920;
	this->height = 1080;
	this->title = " ";
}

Window::Window(int width, int height, const std::string name)
{
	this->width = width;
	this->height = height;
	this->title = name;
	init();
}

/* set window border T/F */
Window::Window(int width, int height, std::string title, bool border)
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

Window::~Window()
{
	destroy();
}
/* closes current glfw window */
void Window::destroy()
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

void Window::init()
{
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);										// Version of GLSL (4.5)
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
	glfwWindowHint(GLFW_SAMPLES, 8);				

	s_keyDispatcher = &this->keyDispatcher;
	s_mouseDispatcher = &this->mouseDispatcher;				
	s_winDispatcher = &this->windowDispatcher;

	glfwSetErrorCallback(setErrCallback);
	glfwSetKeyCallback(window, keyboardHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetCursorPosCallback(window, mousePositionHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetFramebufferSizeCallback(window, windowResizeHandler);

	// GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	// glfwSetCursor(window, cursor);

	// win_icon[0].pixels = stbi_load("./engine/res/red.png", &win_icon[0].width, &win_icon[0].height, 0, 4);
	// if(win_icon[0].pixels)
	// {
	// 	glfwSetWindowIcon(window, 1, win_icon);
	// 	JN_INFO("loaded successfully.");
	// }

	glRender::init(window);
	guiDock.init();
	aspectRatio = this->width / this->height;


}

void Window::onAttach()
{
	
}

void Window::onEvent(const Event& e)
{
	// if(e.getType() == EventType::WINDOW_RESIZE)
	// {
	// 	const WindowEvent& win_event = ((const WindowEvent&)e);
	// }
}

void Window::update(Scene& scene, float dt)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	guiDock.update(scene, dt);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

Dock& Window::getGuiDock()
{
	return guiDock;
}

KeyEventDispatcher& Window::getKeyDispatcher()
{
	return *s_keyDispatcher;
}

MouseEventDispatcher& Window::getMouseDispatcher()
{
	return *s_mouseDispatcher;
}

WindowEventDispatcher& Window::getWinEventDispatcher()
{
	return *s_winDispatcher;
}

bool Window::isRunning()
{
	return !glfwWindowShouldClose(this->window);
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

GLFWwindow* Window::getWindow()
{
	return this->window;
}

