#ifndef CONFIG_HEADER
#define CONFIG_HEADER

#include <Engine/Renderer.hpp>
#include <Engine/Input.hpp>
#include <Engine/ShaderProgram.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/FreeCam.hpp>
#include <Engine/SkyBox.hpp>
#include <Engine/Model.hpp>
#include <Engine/Mesh.hpp>
#include <Engine/DirLight.hpp>
#include <Engine/SpotLight.hpp>
#include <Engine/Screen.hpp>

#include <Engine/OctreeSystem.hpp>

class GameManager : public Engine::GameLoop
{
private:
	Engine::Renderer *renderer;
	Engine::Input *input;

	Engine::ShaderProgram *skyboxProgram;
	Engine::ShaderProgram *objectProgram;
	Engine::ShaderProgram *dirLightProgram;
	Engine::ShaderProgram *spotLightProgram;
	Engine::ShaderProgram *shadowMapProgram;
	Engine::ShaderProgram *backgroundProgram;
	Engine::ShaderProgram *screenProgram;

	Engine::GBuffer *gBuffer;
	Engine::FreeCam *cam;
	Engine::SkyBox *skybox;
	Engine::Model *sol;
	Engine::Model *heli;
	Engine::DirLight *sun;
	Engine::SpotLight *torch;
	Engine::Screen *screen;

	Engine::OctreeSystem *octreeSystem;
public:
	GameManager(Engine::Renderer *renderer, Engine::Input *input);
	~GameManager(void);
	void display(FLOAT state);
	void idle(void);
	void reshape(UINT w, UINT h);
	void launch(void);
};

#endif