#pragma once
#include "pch.h"
#include "render/Window.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "render/shaders/Shader.h"
#include "entity/Entity.h"
#include "render/Mesh.h"
#include "render/textures/Texture.h"
#include "core/Log.h"
#include "core/MathUtils.h"
#include "render/shaders/TerrainShader.h"
#include "entity/terrain/Terrain.h"
#include "render/shaders/SkyBoxShader.h"
#include "entity/SkyBox.h"

#define JN_GFX_DEVICE glGetString(GL_RENDERER)

namespace glRender {

static void checkGLErrors()
{
	while(GLenum err = glGetError())
	{
		JN_CRIT("OpenGL error code: {}"  , err);
	}
	/* clears error queue */
	while(glGetError() != GL_NO_ERROR){}
}

static void init(GLFWwindow* window)
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

/* clear screen */
static void clear()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void resizeFrameBuffer(float width, float height)
{
	glViewport(0, 0, width, height);
}

static void setModeWireframe(bool flag)
{
	if(flag)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
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
static void render(juno::Mesh *m, juno::Shader &shader)
{
	juno::Mesh mesh = *m;
	glBindVertexArray(mesh.getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.getDiffuseTextures()[0].getID());
	/* draw entity */
	//JN_CLI_WARN("RENDERING...");
	glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_INT, 0);
	/* --- */
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	
}

/* for single texture, diffuse-mapped only geometry */
static void renderEntity(juno::Entity& entity, std::shared_ptr<juno::Shader> & shader)
{
	glBindVertexArray(entity.getMesh().getVaoID()); 
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2); 

	glm::mat4 transformationMat = juno::createTransformationMat(entity.getPosition(), entity.getRotation(), entity.getScale());
	shader->loadTransformMatrix(transformationMat);
	shader->loadMaterialVars(entity.getMesh().getMaterial());

	//setBackFaceCulling(!entity.getMesh().getDiffuseTextures()[0].containsTransparency());
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, entity.getMesh().getDiffuseTextures()[0].getID());
	
	glDrawElements(GL_TRIANGLES, entity.getMesh().getNumIndices(), GL_UNSIGNED_INT, 0);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	
	checkGLErrors();
}

static void instancedEntityRender(std::vector<juno::Entity>& entities, std::shared_ptr<juno::Shader>& shader)
{
	glBindVertexArray(entities[0].getMesh().getVaoID());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glm::mat4 transform; 
	shader->loadMaterialVars(entities[0].getMesh().getMaterial());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, entities[0].getMesh().getDiffuseTextures()[0].getID());
	unsigned int num_indices = entities[0].getMesh().getNumIndices();

	for(unsigned int i = 0; i < entities.size(); i++)
	{
		transform = juno::createTransformationMat(entities[i].getPosition(), entities[i].getRotation(), entities[i].getScale());
		shader->loadTransformMatrix(transform);
		glDrawElements(GL_TRIANGLES,num_indices, GL_UNSIGNED_INT, 0);
	}
}


static void renderSkyBox(juno::SkyBox& skybox, std::shared_ptr<juno::SkyBoxShader>& shader)
{
	glDepthMask(GL_FALSE);
	shader->setActive();
	glBindVertexArray(skybox.getVaoID()); 
	glEnableVertexAttribArray(0);		/* vertex positions */
	//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getCubeMap().getID());
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

}


static void renderGui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}





};