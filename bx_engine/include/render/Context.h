#ifndef BXCONTEXT_HEADER
#define BXCONTEXT_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <time.h>
#include "core/Log.h"
#include "core/KeyMap.h"
#include "core/InputStates.h"
#include "render/Camera.h"


namespace bx {

static bool MOUSE_DATA_RECIEVED = false;
static bool LM_BUTTON_PRESS = false;
static bool LM_BUTTON_REPEAT = false;
static bool RM_BUTTON_PRESS = false;
static bool RM_BUTTON_REPEAT = false;
static double CURSOR_X = 0;
static double CURSOR_Y = 0;
static double CURSOR_DX = 0;
static double CURSOR_DY = 0;
static double SCROLL_X = 0;
static double SCROLL_Y = 0;
static double SCROLL_DX = 0;
static double SCROLL_DY = 0;
static double CENTER_X = 0;
static double CENTER_Y = 0;

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
	MouseInputData mouse_data;
};

}

#endif