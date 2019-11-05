#pragma once
#include "pch.h"
#include "render/Context.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "render/shaders/Shader.h"
#include "entity/Entity.h"
#include "render/Mesh.h"
#include "render/textures/Texture.h"
#include "core/Log.h"
#include "core/bxMath.h"
#include "render/shaders/TerrainShader.h"
#include "entity/terrain/Terrain.h"
#include "render/shaders/SkyBoxShader.h"
#include "entity/SkyBox.h"

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
	glEnable(GL_MULTISAMPLE);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	ImGui::StyleColorsDark();


}

/* clear screen */
static void clear()
{
	glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
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

static void renderSkyBox(bx::SkyBox* skybox, std::shared_ptr<bx::SkyBoxShader> & shader)
{
	//glBindVertexArray(skybox->getMesh().getVAO_ID()); 
	glEnableVertexAttribArray(0); 


	//glm::mat4 transformationMat = bxMath::createTransformationMat(entity->getPosition(), entity->getRotation(), entity->getScale());

	/* --- */

	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_CUBE_MAP, );
	// glDrawElements(GL_TRIANGLES, skybox->getNumVertices(), GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
	
}


static void renderGui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// inline static void guiRender()
// {
	
	
// }




static void renderTerrain(bx::Terrain* terrain, std::shared_ptr<bx::TerrainShader> terrain_shader)
{
	glBindVertexArray(terrain->getMesh().getVAO_ID()); 
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2); 

	glm::mat4 transformationMat = bxMath::createTransformationMat(glm::vec3(terrain->getXCoord(), 0, terrain->getZCoord()), glm::vec3(0,0,0),1.0f);
	terrain_shader->loadTransformMatrix(transformationMat);
	terrain_shader->loadPBRVars(terrain->getMesh().getMaterial());
	/* --- */
	setBackFaceCulling(!terrain->getMesh().getDiffuseTextures()[0].containsTransparency());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrain->getMesh().getDiffuseTextures()[0].getID());
	glDrawElements(GL_TRIANGLES, terrain->getMesh().getNumIndices(), GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	
	checkGLErrors();
}



};