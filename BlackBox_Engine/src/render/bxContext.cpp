#include "render/bxContext.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "core/KeyMap.h"
/*
	(OpenGL/Vulkan) Rendering context
*/
using namespace bbx;

bxContext::bxContext()
{
	this->width = 400;
	this->height = 400;
	this->title = " ";
	this->isLoading = true;
	this->isValid = true;

}

bxContext::bxContext(int width, int height, const std::string name)
{
	this->width = width;
	this->height = height;
	this->title = name;
	this->isLoading = true;
	this->isValid = true;
	init();
}

/* set window border */
bxContext::bxContext(int width, int height, std::string title, bool border)
{
	this->width = width;
	this->height = height;
	this->title = title;
	this->isLoading = true;
	this->isValid = true;
	if (!border)
	{
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	}

	init();


}


bxContext::~bxContext()
{
	destroy();
}
/* closes current glfw window */
void bxContext::destroy()
{
	glfwDestroyWindow(window);

}

static void setErrCallback(int code, const char* message)
{
	std::string error(message); // convert char* to string
	BBX_ERR("GLFW window error, code: " + code);
	BBX_CRIT(error);
}



void bxContext::init()
{
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                                  // Oldest allowable version of GLSL (4.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);



	glfwInit();

	if (!glfwInit())
	{
		BBX_ERR("GLFW failed to initialize!");
	}


	this->window = glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL); // third argument makes window full screen if:

	if (!window)
	{
		BBX_ERR("Window creation failed!");
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	glfwSetErrorCallback(setErrCallback);
	glfwSetKeyCallback(window, kbdLayout);

	if (glewInit() != GLEW_OK) { BBX_CRIT("GLEW failed to init!"); }


	// todo: list graphics device through openGL
	BBX_WARN(glGetString(GL_RENDERER));
}

void bxContext::update()
{


	glfwPollEvents();
}

void bxContext::splashImage()
{
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attrib
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);


	Texture bootImage("./res/engine.png");
	glGenerateMipmap(GL_TEXTURE_2D);
	Texture image2("./res/fordo.png");


	glUniform1i(glGetUniformLocation(shaderID, "diffuse"), 0);
	glUniform1i(glGetUniformLocation(shaderID, "normal"), 1);







	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bootImage.getID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, image2.getID());



		//glBindTexture(GL_TEXTURE_2D, bootImage.getID());
		glUseProgram(shaderID);

		glBindVertexArray(VAO);



		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
		glBindTexture(GL_TEXTURE_2D, 0);

	}
}
/*
bool RenderContext::isVisible()
{
	return true;
}
*/

/* sets default shader for context */
void bxContext::setShader(GLuint id)
{
	this->shaderID = id;
}

bool bxContext::isRunning()
{
	return this->isValid;
}
