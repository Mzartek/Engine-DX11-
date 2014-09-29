#include "config.hpp"

// Globales variables
Engine::Renderer *renderer;
Engine::Input *input;
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

void display(FLOAT state)
{
	renderer->clear();
	gBuffer->clear();
	sun->clear();
	torch->clear();

	// Skybox
	skybox->display(gBuffer, cam);

	// Shadow Map
	sol->displayShadowMap(sun);
	heli->displayShadowMap(sun);
	sol->displayShadowMap(torch);
	heli->displayShadowMap(torch);

	// Opaque Object
	sol->display(gBuffer, cam);
	heli->display(gBuffer, cam);
	sun->display(gBuffer, cam);
	torch->display(gBuffer, cam);
	screen->background(gBuffer);

	// Transparent Object
	sol->displayTransparent(gBuffer, cam);
	heli->displayTransparent(gBuffer, cam);
	sun->display(gBuffer, cam);
	torch->display(gBuffer, cam);
	screen->background(gBuffer);

	screen->display(renderer, gBuffer, 1.0f, 1.0f, 1.0f, 1.0f);
}

void idle(void)
{
	input->refresh();
	if (input->getKeyBoardState(DIK_ESCAPE))
		renderer->stopLoop();
	cam->keyboardMove(input->getKeyBoardState(DIK_W), input->getKeyBoardState(DIK_S), input->getKeyBoardState(DIK_A), input->getKeyBoardState(DIK_D));
	cam->mouseMove(input->getMouseRelX(), input->getMouseRelY());
	if (input->getKeyBoardState(DIK_LSHIFT))
		cam->setSpeed(0.05f);
	else
		cam->setSpeed(0.5f);

	if (input->getMouseState(0))
		cam->setSpeed(5.0f);

	heli->matRotate(0.1f, 0, 1, 0);

	cam->position();
	sun->position(heli->getPosition(), 25);
	torch->position();
}

void reshape(UINT width, UINT height)
{
	cam->setPerspective(90.0f, width, height, 0.1f, 1000.0f);
}

void init(void)
{
	cam = new Engine::FreeCam;
	sun = new Engine::DirLight;
	torch = new Engine::SpotLight;
	sol = new Engine::Model;
	heli = new Engine::Model;
	skybox = new Engine::SkyBox;
	screen = new Engine::Screen;
	gBuffer = new Engine::GBuffer;

	objectProgram = new Engine::ShaderProgram;
	dirLightProgram = new Engine::ShaderProgram;
	spotLightProgram = new Engine::ShaderProgram;
	vLightProgram = new Engine::ShaderProgram;
	shadowMapProgram = new Engine::ShaderProgram;
	skyboxProgram = new Engine::ShaderProgram;
	backgroundProgram = new Engine::ShaderProgram;
	screenProgram = new Engine::ShaderProgram;

	configShaders();
	configBuffers();
	configLights();
	configScreen();
	configModels();
	configSkybox();

	cam->setPositionCamera(XMFLOAT3(30, 10, 0));
	cam->setInitialAngle(-90, 0);
}

void kill()
{
	delete screenProgram;
	delete backgroundProgram;
	delete skyboxProgram;
	delete shadowMapProgram;
	delete vLightProgram;
	delete spotLightProgram;
	delete dirLightProgram;
	delete objectProgram;
	
	delete gBuffer;
	delete screen;
	delete skybox;
	delete heli;
	delete sol;
	delete torch;
	delete sun;
	delete cam;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	renderer = new Engine::Renderer;
	input = new Engine::Input;

	renderer->initWindow(hInstance, nCmdShow, TEXT("Demo DirectX"), 800, 600, FALSE);
	renderer->setReshapeFunc(reshape);
	renderer->setIdleFunc(idle);
	renderer->setDisplayFunc(display);

	input->initInput(hInstance, renderer->getHWND());

	init();

	renderer->mainLoop();

	kill();

	delete renderer;
	delete input;

	if (Engine::Object::getMemoryState() != 0)
	{
		std::string text;
		text = std::to_string(Engine::Object::getMemoryState());
		MessageBoxA(NULL, text.c_str(), __FILE__, MB_OK);
	}
	Engine::Object::saveMemoryInfo(TEXT("memLost.txt"));

	return 0;
}