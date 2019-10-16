#pragma once
#include "bxContext.h"
#include <vector>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_ogl3.h"

#include "render/Shader.h"
#include "entity/Entity.h"
#include "render/Mesh.h"
#include "render/Texture.h"
#include "core/Log.h"
#include "core/bxMath.h"

#define BX_GFX_DEVICE glGetString(GL_RENDERER)

namespace bxRender {

	static void init(GLFWwindow* window)
	{
		if (glewInit() != GLEW_OK) { BX_ERR("GLEW failed to init!"); }

		stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);

		// IMGUI_CHECKVERSION();
		// ImGui::CreateContext();
		// ImGuiIO& io = ImGui::GetIO(); (void)io;

		// ImGui::StyleColorsDark();

		// ImGui_ImplGlfw_InitForOpenGL(window, true);
		// ImGui_ImplOpenGL3_Init("#version 410");
	}

	/* clear screen */
	static void clear()
	{
		glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	/* render static mesh */
	static void render(bbx::Mesh *m, bbx::Shader &shader)
	{
		bbx::Mesh mesh = *m;
		glBindVertexArray(mesh.getVAO_ID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.getDiffuseTextures()[0].getID());
		/* draw entity */
		//BX_CLI_WARN("RENDERING...");
		glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
		/* --- */
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
		
	}

	/* render entity */
	static void renderEntity(bbx::Entity& entity, bbx::Shader& shader)
	{
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(entity.getMesh().getVAO_ID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		/* load transformation */
		glm::mat4 transformationMat = bxMath::createTransformationMat(entity.getPosition(), entity.getRotation(), entity.getScale());
		shader.loadTransformMatrix(transformationMat);
		/* --- */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entity.getMesh().getTextureZero().getID());
		glDrawElements(GL_TRIANGLES, entity.getMesh().getNumIndices(), GL_UNSIGNED_INT, 0);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	static void instancedRender(std::vector<bbx::Entity>, bbx::Shader* shader)
	{

		
	}


	// static void guiUpdate()
	// {
	// 	ImGui_ImplOpenGL3_NewFrame();
	// 	ImGui_ImplGlfw_NewFrame();
	// 	ImGui::NewFrame();
	// 	ImGui::Begin("testing");
	// 	ImGui::Text("testing?"); 
		
	// 	ImGui::End();

	// 	ImGui::Render();
	// 	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// }

	// inline static void guiRender()
	// {
		
		
	// }




};