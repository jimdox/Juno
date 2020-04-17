#define JN_PLATFORM_LINUX
#define JN_RENDERER_OPENGL
#include "pch.h"
#include "core/StartPoint.h"
#include "utils/Math.h"
#include "utils/MathUtils.h"
#include "render/shaders/ComputeShader.h"
//#include "igl3D.h"
using namespace juno;

static ComputeShader* g_computeShader;

class SandboxProgram : public Program
{
	void onCreate()
	{
		Mesh plane = AssetManager::get().loadMesh("./engine/res/plane.obj");
		Mesh susy = AssetManager::get().loadMesh("./engine/res/susanne.obj");
		Mesh dragon = AssetManager::get().loadMesh("./engine/res/dragon.obj");

		plane.getMaterial().setBaseColor(Vec3(0.1f, 0.1f, 0.1f));
		plane.getMaterial().setReflectivity(0.12);
		dragon.getMaterial().setBaseColor(Vec3(0.4f, 0.1f, 0.1f));
		dragon.getMaterial().setShineDamper(0.99f);
		dragon.getMaterial().setReflectivity(0.25f);
		
		Entity entityOne (plane, Vec3(0.0f, -5.5f, 0.0f), Vec3(0,0, 0), 100.0f, "default plane");
		Entity entityTwo (susy, Vec3(-30,1, -5), Vec3(-50,40,0), 5.0f, "susanne 0");
		Entity entityThree (dragon, Vec3(60, -5, -30), Vec3(0,-50,0), 5.0f, "Big Boi");
		
		Light light(Vec3(-110.5f, 500.01f, 100.0f), Vec3(0.9f, 0.9f, 0.9f), Vec3(1.0, 1, 0.005));
		Light lightB(Vec3(300,100,-1010), Vec3(0.4f, 0.4f, 0.70f), Vec3(1.0, 0.00001f, 0.0005f));

		// scene.add(entityOne);
		// scene.add(entityTwo);
		// scene.add(entityThree);

		// scene.add(light);
		// scene.add(lightB);
		
		Renderer::get().submit(scene);

		/* sample code for how compute shaders can be used */
		std::vector<Particle> particles;
		for(unsigned int i = 0; i < 100000; i++)
		{
			float x = 100 * sinf(i) * cosf(i) + 60;
			float z = 100 * cosf(i) * cosf(i) - 90;
			float y = 100 * sinf(i * M_PI/300) + 50;
			glm::vec3 position(x,y,z);
			glm::vec3 velocity(sinf(2 * M_PI * i) + cosf(-i), cosf(i/(2 * M_PI)), sinf(2 * M_PI / i));   
			particles.emplace_back(position, velocity, 1.0f);
		}

		g_computeShader = new ComputeShader(particles, "./engine/res/shaders/computeShaders/example", 512);
		

		//renderer->getWindow().getGuiDock().enableDebug();
	}

	/* updates every cycle */
	void onUpdate()
	{
		//delta_x += 0.001f;
	
	}

	/* updates on each new frame */
	void onFrameBufferUpdate()
	{
		Renderer::get().runComputeShader(*g_computeShader, frame_time);

	}


	void onExit()
	{
		/* mem. cleanup */
		delete g_computeShader;
		
	}

private:
	float delta_x = 0.0f;
	float counter = 0;
};








/* --------------------- */
juno::Program* createProgram()
{
 	juno::Program* prog = new SandboxProgram();
 	return prog;
}