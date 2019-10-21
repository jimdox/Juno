#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "core/Log.h"
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

	void destroy();
	void setVisible(bool flag);
	void setShader(GLuint id);

	bool isRunning();

	int getHeight();
	int getWidth();

protected:

private:
	//RenderContext(const RenderContext& other){}
	//RenderContext& operator=(const RenderContext& other){}

	int height;
	int width;
	std::string title;
	GLFWwindow* window;
	GLuint shaderID;
	//GLuint shaders[4];
	bool isLoading;
	bool isValid;

};

}

