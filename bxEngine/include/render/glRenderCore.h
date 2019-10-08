#pragma once
#include "bxContext.h"
#include <vector>
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
#include "render/Shader.h"
#include "entity/Entity.h"
#include "render/Mesh.h"
#include "render/Texture.h"

#include "core/Log.h"


#define BX_GFX_DEVICE glGetString(GL_RENDERER)


namespace bxRender {

	static void init()
	{
		if (glewInit() != GLEW_OK) { BBX_ERR("GLEW failed to init!"); }

		stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);
	}

	/* clear screen */
	static void clear()
	{
		glClearColor(0.4f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	/* render static mesh */
	static void render(bbx::Mesh& mesh, bbx::Shader& shader)
	{
		
		glUniform1i(glGetUniformLocation(shader.getID(), "texture_diffuse1"), 0);
		glBindTexture(GL_TEXTURE_2D, mesh.getDiffuseTextures()[0].getID());

		/* draw entity */
		glBindVertexArray(mesh.getVAO_ID());
		glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		/* --- */
		glActiveTexture(GL_TEXTURE0);
	}

	static void renderEntity(bbx::Entity& entity, bbx::Shader& shader)
	{
		for(unsigned int n = 0; n < entity.getMeshList().size(); n++)
		{
			render(entity.getMeshList()[n], shader);	
		}
	}

	static void instancedRender(std::vector<bbx::Entity>, bbx::Shader* shader)
	{

		
	}
	/*
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
		/* add function calls to bxGUI here 
		ImGui::End();
	}

	inline static void guiRender()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
	}
	*/
	/* todo: move certain functions to debug build only (#ifdef BX_DEBUG) */
	static void testShader()
	{
		



	}

	


};