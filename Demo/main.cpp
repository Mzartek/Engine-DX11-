#include "include/config.hpp"

// Globales variables
engine::Renderer*			renderer = NULL;
engine::Input*				input = NULL;
engine::GBuffer*			gBuffer = NULL;
engine::FreeCam*			cam = NULL;
engine::Model*				sol = NULL;
engine::Model*				heli = NULL;
engine::Screen*				screen = NULL;
engine::SkyBox*				skybox = NULL;
engine::ShaderProgram*		gObjectProgram = NULL;
engine::ShaderProgram*		gSkyboxProgram = NULL;
engine::ShaderProgram*		screenProgram = NULL;

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
	gBuffer->clear();
	renderer->clear();

	skybox->display(gBuffer, cam);
	sol->display(gBuffer, cam);
	heli->display(gBuffer, cam);

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
}

void reshape(UINT width, UINT height)
{
	cam->setPerspective(90.0f, width, height, 0.1f, 1500.0f);
}

void init(void)
{
	gBuffer = new engine::GBuffer;
	cam = new engine::FreeCam;
	sol = new engine::Model;
	heli = new engine::Model;
	screen = new engine::Screen;
	skybox = new engine::SkyBox;
	gObjectProgram = new engine::ShaderProgram;
	gSkyboxProgram = new engine::ShaderProgram;
	screenProgram = new engine::ShaderProgram;

	configShader();
	configBuffer();
	configModels();
	configScreen();
	configSkyBox();

	cam->setPositionCamera(XMFLOAT3(30, 10, 0));
	cam->setInitialAngle(-90, 0);
}

void kill()
{
	delete screenProgram;
	delete gSkyboxProgram;
	delete gObjectProgram;
	delete skybox;
	delete screen;
	delete heli;
	delete sol;
	delete cam;
	delete gBuffer;
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