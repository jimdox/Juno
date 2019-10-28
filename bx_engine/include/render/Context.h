#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "core/Log.h"
#include "core/KeyMap.h"
#include "core/MouseMap.h"
#include "render/Camera.h"


namespace bx {
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
	//void splashImage();
	void update();
	void updateCamera(Camera* camera, float dt);
	void destroy();
	void setVisible(bool flag);

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

	//GLuint shaders[4];
	bool isLoading;
	bool isValid;

};

}

