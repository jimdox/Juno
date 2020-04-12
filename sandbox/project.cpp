#define JN_PLATFORM_LINUX
#define JN_RENDERER_OPENGL
#include "core/StartPoint.h"
#include "utils/Math.h"

//#include "igl3D.h"
using namespace juno;


class SandboxProgram : public juno::Program
{
	void onCreate()
	{
		Mesh susy = AssetManager::get().loadMesh("./juno_engine/res/susanne.obj");
		
		Entity entity_one(susy, Vec3(0.0f, 2.8f, -8.0f), Vec3(-10,-40,10), 10.0f, "susanne 1");
		
		Shader shader1 = AssetManager::get().loadShader("./juno_engine/res/shaders/skybox");
		
		
		Mesh plane = AssetManager::get().loadMesh("./juno_engine/res/plane.obj");
		
		plane.getMaterial().setBaseColor(Vec3(0.12f, 0.12f, 0.12f));
		plane.getMaterial().setReflectivity(0.12);
		Entity entity_two(plane, Vec3(0,-10,0), Vec3(0,0,0), 100.0f, "Default Plane");
		Entity entity_three(susy, Vec3(-30,1, -5), Vec3(-50,40,0), 5.0f, "susanne 2");
		Entity entity_four(susy, Vec3(60, 20, -30), Vec3(0,-50,0), 30.0f, "Big Boi");


		Light light(Vec3(-110.5f, 500.01f, 100.0f), Vec3(0.9f, 0.9f, 0.9f), Vec3(1.0, 1, 0.005));
		Light light_b(Vec3(300,100,10), Vec3(0.4f, 0.4f, 0.70f), Vec3(1.0, 0.00001f, 0.0005f));


		scene.addEntity(entity_one);
		scene.addEntity(entity_two);
		scene.addEntity(entity_three);
		scene.addEntity(entity_four);

		scene.addLight(light);
		scene.addLight(light_b);
		renderer->submit(&scene);

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
		/* update display info */
		//renderer->getWindow().getGuiDock().printToDebug("info %f", delta_x);
		//scene.getEntities()[0].addPosition(glm::vec3(delta_x, 0, 0));
		delta_x = 0;
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