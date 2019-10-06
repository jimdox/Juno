#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "core/Log.h"
namespace bbx {
class bxContext
{
public:
	bxContext();
	bxContext(int width, int height, std::string title);
	bxContext(int width, int height, std::string title, bool border);
	virtual ~bxContext();
	GLFWwindow* getWindow();
	void renderUpdate();
	void init();
	void splashImage();
	void update();

	void destroy();
	void setVisible(bool flag);
	void setShader(GLuint id);

	bool isRunning();



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

