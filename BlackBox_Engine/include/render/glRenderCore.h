#pragma once
#include <GL/glew.h>
#include <vector>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "render/Shader.h"
#include "entity/Entity.h"
#include "render/Mesh.h"
#include "render/Texture.h"

#include "core/Log.h"


#define BX_GFX_DEVICE glGetString(GL_RENDERER)

namespace bxRender {

	static void init()
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		stbi_set_flip_vertically_on_load(true);


		glEnable(GL_DEPTH_TEST);
	}

	/* clear screen */
	static void clear()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	/* render static mesh */
	static void render(bbx::Entity)
	{
		
	}

	static void instancedRender(std::vector<bbx::Entity>, bbx::Shader* shader)
	{

		
	}

	static void initGUI(GLFWwindow* window, unsigned int glslVersion)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		std::string glslVer = "#version " + glslVersion;
		ImGui_ImplOpenGL3_Init("#version 400");
		
	}

	inline static void guiUpdate()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("testing");
		ImGui::Text("texting?"); /* sample code */
		/* add function calls to bxGUI here */
		ImGui::End();
	}

	inline static void guiRender()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
	}

	


};