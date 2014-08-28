#include "include/config.hpp"

// Globales variables
engine::Renderer *renderer;
engine::Input *input;
engine::FreeCam *cam;
engine::DirLight *sun;
engine::SpotLight *torch;
engine::Model *sol;
engine::Model *heli;
engine::SkyBox *skybox;
engine::Screen *screen;
engine::GBuffer *gBuffer;

engine::ShaderProgram *gObjectProgram;
engine::ShaderProgram *dirLightProgram;
engine::ShaderProgram *spotLightProgram;
engine::ShaderProgram *shadowProgram;
engine::ShaderProgram *gSkyboxProgram;
engine::ShaderProgram *screenProgram;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

void display(void)
{
	renderer->clear();
	gBuffer->clear();
	sun->clear();
	torch->clear();

	skybox->display(gBuffer, cam);
	sol->display(gBuffer, cam);
	heli->display(gBuffer, cam);

	sol->displayShadow(sun);
	heli->displayShadow(sun);

	sol->displayShadow(torch);
	heli->displayShadow(torch);

	sun->display(gBuffer, cam);
	//torch->display(gBuffer, cam);

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
		cam->setSpeed(0.25f);

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
	cam = new engine::FreeCam;
	sun = new engine::DirLight;
	torch = new engine::SpotLight;
	sol = new engine::Model;
	heli = new engine::Model;
	skybox = new engine::SkyBox;
	screen = new engine::Screen;
	gBuffer = new engine::GBuffer;

	gObjectProgram = new engine::ShaderProgram;
	dirLightProgram = new engine::ShaderProgram;
	spotLightProgram = new engine::ShaderProgram;
	shadowProgram = new engine::ShaderProgram;
	gSkyboxProgram = new engine::ShaderProgram;
	screenProgram = new engine::ShaderProgram;

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
	delete gSkyboxProgram;
	delete shadowProgram;
	delete spotLightProgram;
	delete dirLightProgram;
	delete gObjectProgram;

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

	renderer = new engine::Renderer;
	input = new engine::Input;

	renderer->initWindow(hInstance, WndProc, "Demo DirectX", 800, 600, FALSE);
	renderer->setReshapeFunc(reshape);
	renderer->setIdleFunc(idle);
	renderer->setDisplayFunc(display);

	input->initInput(hInstance, renderer->getHWND());

	init();

	renderer->mainLoop(nCmdShow);

	kill();

	delete renderer;
	delete input;

	if (engine::Object::getMemoryState() != 0)
	{
		std::string text;
		text = std::to_string(engine::Object::getMemoryState());
		MessageBox(NULL, text.c_str(), "memState", MB_OK);
	}
	engine::Object::saveMemoryInfo(L"memLost.txt");

	return 0;
}