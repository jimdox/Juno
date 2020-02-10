#ifndef JNCONTEXT_HEADER
#define JNCONTEXT_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "core/Log.h"
#include "core/InputStates.h"
#include "render/Camera.h"
#include "gui/Dock.h"
#include "events/MouseEvent.h"
#include "events/KeyEvent.h"

namespace juno {


class Context
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

	KeyEventDispatcher& getEventDispatcher();

	bool isRunning();
	int getHeight();
	int getWidth();

private:
	//RenderContext(const RenderContext& other){}
	//RenderContext& operator=(const RenderContext& other){}
	GLFWwindow* window;
	int height;
	int width;
	std::string title;
	Dock guiDock;

	KeyEventDispatcher eventDispatcher;
	bool isLoading;
	bool isValid;
};

}

#endif