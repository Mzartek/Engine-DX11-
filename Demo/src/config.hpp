#ifndef GRAPHICS
#define GRAPHICS

#include <Engine/Renderer.hpp>
#include <Engine/Input.hpp>
#include <Engine/FreeCam.hpp>
#include <Engine/DirLight.hpp>
#include <Engine/SpotLight.hpp>
#include <Engine/Model.hpp>
#include <Engine/SkyBox.hpp>
#include <Engine/Screen.hpp>
#include <Engine/GBuffer.hpp>
#include <Engine/ShaderProgram.hpp>

extern Engine::Renderer *renderer;
extern Engine::Input *input;

class GameManager : public Engine::GameLoop
{
private:
	Engine::FreeCam *cam;
	Engine::DirLight *sun;
	Engine::SpotLight *torch;
	Engine::Model *sol;
	Engine::Model *heli;
	Engine::SkyBox *skybox;
	Engine::Screen *screen;
	Engine::GBuffer *gBuffer;

	Engine::ShaderProgram *objectProgram;
	Engine::ShaderProgram *dirLightProgram;
	Engine::ShaderProgram *spotLightProgram;
	Engine::ShaderProgram *vLightProgram;
	Engine::ShaderProgram *shadowMapProgram;
	Engine::ShaderProgram *skyboxProgram;
	Engine::ShaderProgram *backgroundProgram;
	Engine::ShaderProgram *screenProgram;
	Engine::ShaderProgram *textProgram;
public:
	GameManager(void);
	~GameManager(void);
	void display(FLOAT state);
	void idle(void);
	void reshape(UINT w, UINT h);
};

#endif