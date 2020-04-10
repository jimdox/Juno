#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "core/Log.h"
#include "gui/Dock.h"
#include "events/MouseEvent.h"
#include "events/KeyEvent.h"
#include "events/WindowEvent.h"

namespace juno {

class Window : public EventListener
{
public:
	Window();
	Window(int width, int height, std::string title);
	Window(int width, int height, std::string title, bool border);
	virtual ~Window();

	void onAttach();
	void onEvent(const Event& e);

	GLFWwindow* getWindow();
	void renderUpdate();
	void init();
	void update(Scene& scene, float dt);
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
	Window(const Window& other) {}

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
