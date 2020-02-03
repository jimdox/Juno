#ifndef JNCONTEXT_HEADER
#define JNCONTEXT_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "core/Log.h"
#include "core/KeyMap.h"
#include "core/InputStates.h"
#include "render/Camera.h"
#include "gui/Dock.h"
#include "events/MouseEvent.h"
#include "events/KeyEvent.h"

namespace juno {


class Context : EventDispatcher
{
public:
	Context();
	Context(int width, int height, std::string title);
	Context(int width, int height, std::string title, bool border);
	virtual ~Context();
	GLFWwindow* getWindow();
	void renderUpdate();
	void init();
	void update(float dt);
	void updateCamera(Camera* camera, float dt);
	void destroy();
	void setVisible(bool flag);

	void attachListener(EventListener* listener);
	void detatchListener(EventListener* listener);
	void dispatchEvent(const Event& e);

	bool isRunning();
	int getHeight();
	int getWidth();

protected:

private:
	//RenderContext(const RenderContext& other){}
	//RenderContext& operator=(const RenderContext& other){}
	GLFWwindow* window;
	int height;
	int width;
	std::string title;
	Dock guiDock;

	//GLuint shaders[4];
	bool isLoading;
	bool isValid;
};

}

#endif