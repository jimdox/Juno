#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "render/Texture.h"
#include "core/Log.h"
namespace bbx {
class RenderContext
{
public:
	RenderContext();
	RenderContext(int width, int height, std::string title);
	RenderContext(int width, int height, std::string title, bool border);
	virtual ~RenderContext();
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

