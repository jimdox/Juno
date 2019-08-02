#pragma once
#include <GL/GL.h>
#include <vector>
#include "render/Mesh.h"
#include "entity/Entity.h"
#include "core/Log.h"
#define BX_GFX_DEVICE glGetString(GL_RENDERER)

namespace bxRender {

	/* clear screen */
	static void clear()
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	/* render static mesh */
	static void render(bbx::Entity)
	{
		//int n = mesh->getNumVertices();

	}

	static void instancedRender(std::vector<bbx::Entity>)
	{

	}

	static void getHWInfo()
	{
		BBX_WARN(glGetString(GL_RENDERER)); // list video card

	}


};