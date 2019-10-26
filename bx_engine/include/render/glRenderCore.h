#pragma once
#include "Context.h"
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_ogl3.h"

#include "render/shaders/Shader.h"
#include "entity/Entity.h"
#include "render/Mesh.h"
#include "render/Texture.h"
#include "core/Log.h"
#include "core/bxMath.h"
#include <vector>
#include "render/shaders/TerrainShader.h"
#include "entity/terrain/Terrain.h"

#define BX_GFX_DEVICE glGetString(GL_RENDERER)

namespace bxRender {

	static void checkGLErrors()
	{
		while(GLenum err = glGetError())
		{
			BX_CRIT("OpenGL error code: {}"  , err);
		}

		while(glGetError() != GL_NO_ERROR){}
	}

	static void init(GLFWwindow* window)
	{
		if (glewInit() != GLEW_OK) { BX_ERR("GLEW failed to init!"); }

		stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

	static void setBackFaceCulling(bool flag)
	{
		if(flag)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		} else {
			glDisable(GL_CULL_FACE);
		}
	}


	/* render static mesh */
	static void render(bx::Mesh *m, bx::Shader &shader)
	{
		bx::Mesh mesh = *m;
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
	static void renderEntity(bx::Entity* entity, std::shared_ptr<bx::Shader> & shader)
	{
		glBindVertexArray(entity->getMesh().getVAO_ID()); 
		glEnableVertexAttribArray(0); 
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2); 

		glm::mat4 transformationMat = bxMath::createTransformationMat(entity->getPosition(), entity->getRotation(), entity->getScale());
		shader->loadTransformMatrix(transformationMat);
		shader->loadPBRVars(entity->getMesh().getMaterial());
		/* --- */
		setBackFaceCulling(!entity->getMesh().getDiffuseTextures()[0].containsTransparency());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, entity->getMesh().getDiffuseTextures()[0].getID());
		glDrawElements(GL_TRIANGLES, entity->getMesh().getNumIndices(), GL_UNSIGNED_INT, 0);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
		
		checkGLErrors();
	}

	static void batchRender(std::vector<bx::Entity> &entities, std::shared_ptr<bx::Shader> & shader)
	{
		bx::Mesh& mesh = entities[0].getMesh();
		glBindVertexArray(mesh.getVAO_ID()); 
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2); 
		shader->loadPBRVars(mesh.getMaterial());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.getDiffuseTextures()[0].getID());

		for(int i = 0; i < entities.size(); i++)
		{
		glm::mat4 transformationMat = bxMath::createTransformationMat(entities[i].getPosition(), entities[i].getRotation(), entities[i].getScale());
		shader->loadTransformMatrix(transformationMat);
		glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_INT, 0);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindVertexArray(0);
		
		checkGLErrors();
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




	// static void renderTerrain(bx::Terrain* terrain, std::shared_ptr<bx::Shader> shader)
	// {
	// 	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

	// 	glBindVertexArray(terrain->getMesh().getVAO_ID()); 
	// 	glEnableVertexAttribArray(0); 
	// 	glEnableVertexAttribArray(1);
	// 	glEnableVertexAttribArray(2); 

	// 	glm::mat4 transformationMat = bxMath::createTransformationMat(glm::vec3(terrain->getXCoord(), 0, terrain->getZCoord()), glm::vec3(0,0,0),1.0f);
	// 	shader->loadTransformMatrix(transformationMat);
	// 	shader->loadPBRVars(terrain->getMesh().getMaterial());
	// 	/* --- */
	// 	setBackFaceCulling(!terrain->getMesh().getDiffuseTextures()[0].containsTransparency());

	// 	glActiveTexture(GL_TEXTURE0);
	// 	glBindTexture(GL_TEXTURE_2D, terrain->getMesh().getDiffuseTextures()[0].getID());
	// 	glDrawElements(GL_TRIANGLES, terrain->getMesh().getNumIndices(), GL_UNSIGNED_INT, 0);
		
	// 	glDisableVertexAttribArray(0);
	// 	glDisableVertexAttribArray(1);
	// 	glDisableVertexAttribArray(2);
	// 	glBindVertexArray(0);
		
	// 	checkGLErrors();
	// }



};