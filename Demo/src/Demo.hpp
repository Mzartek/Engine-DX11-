#ifndef DEMO_HEADER
#define DEMO_HEADER

#include <Engine/Engine.hpp>

class Demo : public Engine::GameLoop
{
private:
	Engine::Renderer *renderer;
	Engine::Input *input;

	Engine::ShaderProgram *skyboxProgram;
	Engine::ShaderProgram *objectProgram;
	Engine::ShaderProgram *reflectObjectProgram;
	Engine::ShaderProgram *dirLightProgram;
	Engine::ShaderProgram *spotLightProgram;
	Engine::ShaderProgram *depthMapProgram;
	Engine::ShaderProgram *backgroundProgram;
	Engine::ShaderProgram *screenProgram;

	Engine::GBuffer *gBuffer;
	Engine::DepthMap *dMaps;
	Engine::FreeCam *cam;
	Engine::SkyBox *skybox;
	Engine::Model *sol;
	Engine::Model *heli;
	Engine::DirLight *sun;
	Engine::SpotLight *torch;
	Engine::Screen *screen;

	Engine::OctreeSystem *octreeSystem;
public:
	Demo(Engine::Renderer *renderer, Engine::Input *input);
	~Demo(void);
	void display(FLOAT state);
	void idle(long long time);
	void reshape(UINT w, UINT h);
	void launch(void);
};

#endif