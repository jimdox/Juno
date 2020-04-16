#define JN_PLATFORM_LINUX
#define JN_RENDERER_OPENGL
#include "core/StartPoint.h"
#include "utils/Math.h"
#include "render/shaders/ComputeShader.h"
//#include "igl3D.h"
using namespace juno;


class SandboxProgram : public juno::Program
{
	void onCreate()
	{
		// Mesh plane = AssetManager::get().loadMesh("./engine/res/plane.obj");
		// Mesh susy = AssetManager::get().loadMesh("./engine/res/susanne.obj");
		// Mesh dragon = AssetManager::get().loadMesh("./engine/res/dragon.obj");

		// plane.getMaterial().setBaseColor(Vec3(0.1f, 0.1f, 0.1f));
		// plane.getMaterial().setReflectivity(0.12);
		// dragon.getMaterial().setBaseColor(Vec3(0.4f, 0.1f, 0.1f));
		// dragon.getMaterial().setShineDamper(0.99f);
		// dragon.getMaterial().setReflectivity(0.25f);
		
		// Entity entityOne (plane, Vec3(0.0f, -20.0f, 0.0f), Vec3(0,0, 0), 100.0f, "default plane");
		// Entity entityTwo (susy, Vec3(-30,1, -5), Vec3(-50,40,0), 5.0f, "susanne 2");
		// Entity entityThree (dragon, Vec3(60, -5, -30), Vec3(0,-50,0), 5.0f, "Big Boi");
		
		// Light light(Vec3(-110.5f, 500.01f, 100.0f), Vec3(0.9f, 0.9f, 0.9f), Vec3(1.0, 1, 0.005));
		// Light lightB(Vec3(300,100,-1010), Vec3(0.4f, 0.4f, 0.70f), Vec3(1.0, 0.00001f, 0.0005f));

		// scene.addEntity(entityOne);
		// scene.addEntity(entityTwo);
		// scene.addEntity(entityThree);
		// scene.addLight(light);
		// scene.addLight(lightB);
		renderer->submit(&scene);












		/* sample code for how compute shaders might used by application -- not currently implemented */
		// std::vector<Particle> particles;

		// for(unsigned int i = 0; i < 1000; i++)
		// {
		// 	Particle p;
		// 	p.position = glm::vec3( sinf(i)/(min(i, 50000)) * 5000, cosf(i)/(min(i, 50000)) * 5000, (tanf(-i * M_PI + M_PI*2)/i) * 1000);
		// 	p.velocity = glm::vec3(100, 100, 100);
		// 	p.mass = 0.1f;
		// 	p.scale = 1.0f;
		// 	particles.emplace_back(p);
		// }

		// ComputeShader cs = AssetManager::get().loadComputeShader("/path-to-shader-file");
		// renderer->submit(cs);














		//renderer->getWindow().getGuiDock().enableDebug();
		/* simulation setup */
	}

	void onDestroy()
	{
		/* mem. cleanup */

	}

	void onUpdate()
	{
		/* update physics */
		delta_x += 0.001f;
	
	}

	void onFrameBufferUpdate()
	{
		//renderer->runComputeShader(delta_x);
		/* update display info */
		//renderer->getWindow().getGuiDock().printToDebug("info %f", delta_x);
		//scene.getEntities()[0].addPosition(glm::vec3(delta_x, 0, 0));
		//delta_x = 0;
	
	}

private:
	float delta_x = 0.0f;
	float counter = 0;

};



juno::Program* createProgram()
{
 	juno::Program* prog = new SandboxProgram();
 	return prog;
}