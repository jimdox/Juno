#include "jnpch.h"
#include "Renderer/Window.h"
#include "Renderer/RenderCoreGL.h"
#include "Core/Config.h"

#ifdef JN_RENDERER_OPENGL
	#include "RenderCoreGL.h"
#elseif JN_RENDERER_VULKAN
	#include "RenderCoreVK.h"
#endif

/*
	Rendering context: manages the window, initialization of rendering environment. 
*/
using namespace Juno;

/* These get passed to glfw callbacks to get IO events */
static KeyEventDispatcher* s_keyDispatcher;
static MouseEventDispatcher* s_mouseDispatcher;
static WindowEventDispatcher* s_winDispatcher;

/* /// GLFW Callbacks /// */

static void KeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(!ImGui::GetIO().WantCaptureKeyboard)										/* check if imgui is using keyboard input */
	{
		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			s_keyDispatcher->Notify(KeyPressEvent(key, 0));						/* todo: key repeat counting */
		} else {
			s_keyDispatcher->Notify(KeyReleaseEvent(key));
		}
	}
}

static void MousePositionHandler(GLFWwindow* window, double x_pos, double y_pos)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	s_mouseDispatcher->Notify(MouseMoveEvent(x_pos - width/2, y_pos - height/2));
	//JN_CRIT("Mouse is moving!");
}

static void MouseButtonHandler(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		switch(button)
		{
		case GLFW_MOUSE_BUTTON_1:
			s_mouseDispatcher->Notify(MousePressEvent(MouseCode::M_BUTTON_LEFT));
			break;
		case GLFW_MOUSE_BUTTON_3:
			s_mouseDispatcher->Notify(MousePressEvent(MouseCode::M_BUTTON_MID));
			break;
		case GLFW_MOUSE_BUTTON_2:
			s_mouseDispatcher->Notify(MousePressEvent(MouseCode::M_BUTTON_RIGHT));
			break;
		default:								/* discard all other mouse codes */
			break;
		}
	} else {					
		switch(button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
				s_mouseDispatcher->Notify(MouseReleaseEvent(MouseCode::M_BUTTON_LEFT));
				break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			s_mouseDispatcher->Notify(MouseReleaseEvent(MouseCode::M_BUTTON_MID));
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			s_mouseDispatcher->Notify(MouseReleaseEvent(MouseCode::M_BUTTON_RIGHT));
			break;
		default:								
			break;
		}
	}
}

static void MouseScrollHandler(GLFWwindow* window, double x_offset, double y_offset)
{
	s_mouseDispatcher->Notify(MouseScrollEvent(x_offset, y_offset));
}

void WindowResizeHandler(GLFWwindow* window, int width, int height)
{
	s_winDispatcher->Notify(WindowResizeEvent(width, height));
}	

static void ErrorCallback(int code, const char* message)
{
	std::string error(message);
	JN_ERR("GLFW window error code: " + code);
	JN_CRIT(error);
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
	Init();
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
	Init();
}

Window::~Window()
{
	Destroy();
}
/* closes current glfw window */
void Window::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwDestroyWindow(window);
}



void Window::Init()
{
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);										// Version of GLSL (4.5)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
	glfwWindowHint(GLFW_SAMPLES, 8);				

	s_keyDispatcher = &this->keyDispatcher;
	s_mouseDispatcher = &this->mouseDispatcher;				
	s_winDispatcher = &this->windowDispatcher;

	glfwSetErrorCallback(ErrorCallback);
	glfwSetKeyCallback(window, KeyboardHandler);
	glfwSetMouseButtonCallback(window, MouseButtonHandler);
	glfwSetCursorPosCallback(window, MousePositionHandler);
	glfwSetScrollCallback(window, MouseScrollHandler);
	glfwSetFramebufferSizeCallback(window, WindowResizeHandler);

	// GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	// glfwSetCursor(window, cursor);

	// win_icon[0].pixels = stbi_load(" ", &win_icon[0].width, &win_icon[0].height, 0, 4);
	// if(win_icon[0].pixels)
	// {
	// 	glfwSetWindowIcon(window, 1, win_icon);
	// 	JN_INFO("loaded successfully.");
	// }

	RenderCall::Init(window);
	guiDock.Init();
	// aspectRatio = this->width / this->height;


}

void Window::OnAttach()
{
	
}

void Window::OnEvent(const Event& e)
{
	// if(e.getType() == EventType::WINDOW_RESIZE)
	// {
	// 	const WindowEvent& win_event = ((const WindowEvent&)e);
	// }
}

void Window::Update(Scene& scene, float dt)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	guiDock.Update(scene, dt);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

Dock& Window::GetGuiDock()
{
	return guiDock;
}

KeyEventDispatcher& Window::GetKeyDispatcher()
{
	return *s_keyDispatcher;
}

MouseEventDispatcher& Window::GetMouseDispatcher()
{
	return *s_mouseDispatcher;
}

WindowEventDispatcher& Window::GetWinEventDispatcher()
{
	return *s_winDispatcher;
}

bool Window::IsRunning()
{
	return !glfwWindowShouldClose(this->window);
}

int Window::GetWidth()
{
	return width;
}

int Window::GetHeight()
{
	return height;
}

GLFWwindow* Window::GetWindow()
{
	return this->window;
}

