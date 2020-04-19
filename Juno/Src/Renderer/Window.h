#pragma once
#include <string>
#include <iostream>
#include <time.h>
#include "Core/Log.h"
#include "Events/MouseEvents.h"
#include "Events/KeyEvents.h"
#include "Events/WindowEvents.h"
// #define GLFW_INCLUDE_VULKAN
#include <glad/glad.h>
#include <GLFW/glfw3.h>


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
	void Update();
	void Destroy();
	void SetVisible(bool flag);

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

	int height;
	int width;
	std::string title;

	KeyEventDispatcher keyDispatcher;
	MouseEventDispatcher mouseDispatcher;
	WindowEventDispatcher windowDispatcher;
};
}
