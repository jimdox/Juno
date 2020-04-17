#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "Core/Log.h"
#include "Gui/Dock.h"
#include "Core/Scene.h"
#include "Events/MouseEvents.h"
#include "Events/KeyEvents.h"
#include "Events/WindowEvents.h"

namespace Juno 
{
class Window : public EventListener
{
public:
	Window();
	Window(int width, int height, std::string title);
	Window(int width, int height, std::string title, bool border);
	virtual ~Window();

	void OnAttach();
	void OnEvent(const Event& e);

	GLFWwindow* GetWindow();
	void Init();
	void Update(Scene& scene, float dt);
	void Destroy();
	void SetVisible(bool flag);
	Dock& GetGuiDock();

	static KeyEventDispatcher& GetKeyDispatcher();
	static MouseEventDispatcher& GetMouseDispatcher();
	static WindowEventDispatcher& GetWinEventDispatcher();

	bool IsRunning();
	int GetHeight();
	int GetWidth();

private:
	Window(const Window& other) {}
	GLFWwindow* window;
	GLFWimage winIcon[1];

	Dock guiDock;
	int height;
	int width;
	std::string title;

	KeyEventDispatcher keyDispatcher;
	MouseEventDispatcher mouseDispatcher;
	WindowEventDispatcher windowDispatcher;
};
}
