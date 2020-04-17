#pragma once
#include "Renderer/Window.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer/Shaders/Shader.h"
#include "Renderer/Shaders/ComputeShader.h"

#include "Entities/Entity.h"
#include "Renderer/Mesh.h"
#include "Renderer/Textures/Texture.h"
#include "Core/Log.h"
#include "Utils/Math.h"
//#include "Renderer/Shaders/TerrainShader.h"
#include "Entities/Terrain/Terrain.h"
//#include "Renderer/Shaders/SkyBoxShader.h"
#include "Entities/SkyBox.h"

#define JN_GFX_DEVICE glGetString(GL_RENDERER)

namespace RenderCall {



static void Init(GLFWwindow* window)
{
	if (glewInit() != GLEW_OK)
	{
		JN_ERR("GLEW failed to init!"); 
	}
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
}

static void ErrorCheck()
{
	while(GLenum err = glGetError())
	{
		JN_CRIT("OpenGL error code: {}"  , err);
	}
	/* clears error queue */
	while(glGetError() != GL_NO_ERROR){}
}

/* clear screen */
static void Clear()
{
	glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void ResizeFrameBuffer(float width, float height)
{
	glViewport(0, 0, width, height);
}

static void SetBackFaceCulling(bool flag)
{
	if(flag)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	} else {
		glDisable(GL_CULL_FACE);
	}
}

static void SetModeWireframe(bool flag)
{
	if(flag)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		SetBackFaceCulling(false);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		SetBackFaceCulling(true);
	}
}

/* render static mesh */
static void Render(Juno::Mesh *m, Juno::Shader &shader)
{
	// juno::Mesh mesh = *m;
	// glBindVertexArray(mesh.getVaoID());
	// glEnableVertexAttribArray(0);
	// glEnableVertexAttribArray(1);
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, mesh.getDiffuseTextures()[0].getID());
	// /* draw entity */
	// //JN_CLI_WARN("RENDERING...");
	// glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_INT, 0);
	// /* --- */
	// glDisableVertexAttribArray(0);
	// glDisableVertexAttribArray(1);
	// glBindVertexArray(0);
	
}

/* for single texture, diffuse-mapped only geometry */
static void RenderEntity(Juno::Entity& entity, Juno::Shader& shader)
{
	glBindVertexArray(entity.GetMesh().GetVaoID()); 
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2); 

	glm::mat4 transformationMat = Juno::CreateTransformationMatrix(entity.GetPosition(), entity.GetRotation(), entity.GetScale());
	shader.LoadTransformMatrix(transformationMat);
	shader.LoadMaterialVars(entity.GetMesh().GetMaterial());

	//setBackFaceCulling(!entity.getMesh().getDiffuseTextures()[0].containsTransparency());
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, entity.getMesh().getDiffuseTextures()[0].getID());
	
	glDrawElements(GL_TRIANGLES, entity.GetMesh().GetNumIndices(), GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	
	ErrorCheck();
}

static void instancedEntityRender(std::vector<Juno::Entity>& entities, std::shared_ptr<Juno::Shader>& shader)
{
	glBindVertexArray(entities[0].GetMesh().GetVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glm::mat4 transform; 
	shader->LoadMaterialVars(entities[0].GetMesh().GetMaterial());
	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, entities[0].getMesh().getDiffuseTextures()[0].getID());
	unsigned int num_indices = entities[0].GetMesh().GetNumIndices();

	for(unsigned int i = 0; i < entities.size(); i++)
	{
		transform = Juno::CreateTransformationMatrix(entities[i].GetPosition(), entities[i].GetRotation(), entities[i].GetScale());
		shader->LoadTransformMatrix(transform);
		glDrawElements(GL_TRIANGLES,num_indices, GL_UNSIGNED_INT, 0);
	}
}


// static void RenderSkyBox(juno::SkyBox& skybox, std::shared_ptr<juno::SkyBoxShader>& shader)
// {
// 	glDepthMask(GL_FALSE);
// 	shader->setActive();
// 	glBindVertexArray(skybox.getVaoID()); 
// 	glEnableVertexAttribArray(0);		/* vertex positions */
// 	//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getCubeMap().getID());
	
// 	glDrawArrays(GL_TRIANGLES, 0, 36);
	
// 	glDisableVertexAttribArray(0);
// 	glBindVertexArray(0);
// 	glDepthMask(GL_TRUE);

// }


static void RenderGui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// static void usingComputeShader(juno::ComputeShader& compShader);
// {

// 	/* ------------------------ */

	
// }

static void ApplyComputeShader(Juno::ComputeShader& cs)
{

    glDrawArraysInstanced(GL_POINTS, 0, 1, cs.GetNumObjects());
    glBindVertexArray(0);
    glUseProgram(0);
}

}
