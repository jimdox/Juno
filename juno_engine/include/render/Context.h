#pragma once
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

class Context : public EventListener
{
public:
	Context();
	Context(int width, int height, std::string title);
	Context(int width, int height, std::string title, bool border);
	virtual ~Context();

	void onAttach();
	void onEvent(const Event& e);

	GLFWwindow* getWindow();
	void renderUpdate();
	void init();
	void update(float dt);
	void updateCamera(Camera* camera, float dt);
	void destroy();
	void setVisible(bool flag);
	Dock& getGuiDock();

	static KeyEventDispatcher& getKeyDispatcher();
	static MouseEventDispatcher& getMouseDispatcher();
	static WindowEventDispatcher& getWinEventDispatcher();

	bool isRunning();
	int getHeight();
	int getWidth();


private:
	Context(const Context& other) {}

	GLFWwindow* window;
	int height;
	int width;
	std::string title;
	Dock guiDock;

	KeyEventDispatcher keyDispatcher;
	MouseEventDispatcher mouseDispatcher;
	WindowEventDispatcher windowDispatcher;
};
}
