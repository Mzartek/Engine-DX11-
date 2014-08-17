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
	cam->position();

	gBuffer->clear();
	skybox->display(gBuffer, cam);
	heli->display(gBuffer, cam);
	sol->display(gBuffer, cam);
	renderer->executeDeferredContext(gBuffer->getContext());

	renderer->clear();
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

	if (FAILED(configShader()))
	{
		MessageBox(NULL, "Error Config Shader", "Main", MB_OK);
		exit(1);
	}
	if (FAILED(configBuffer()))
	{
		MessageBox(NULL, "Error Config Buffer", "Main", MB_OK);
		exit(1);
	}
	if (FAILED(configModels()))
	{
		MessageBox(NULL, "Error Config Models", "Main", MB_OK);
		exit(1);
	}
	if (FAILED(configScreen()))
	{
		MessageBox(NULL, "Error Config Screen", "Main", MB_OK);
		exit(1);
	}
	if (FAILED(configSkyBox()))
	{
		MessageBox(NULL, "Error Config SkyBox", "Main", MB_OK);
		exit(1);
	}

	cam->setPositionCamera(30, 5, 0);
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

	std::string text;
	renderer = new engine::Renderer;
	input = new engine::Input;

	if (FAILED(renderer->initWindow(hInstance, WndProc, "Demo DirectX", 800, 600, FALSE)))
	{
		MessageBox(NULL, "Error while init window", "Error", MB_OK);
		return 1;
	}
	if (FAILED(input->initInput(hInstance, renderer->getHWND())))
	{
		MessageBox(NULL, "Error while init input", "Error", MB_OK);
		return 1;
	}
	renderer->setReshapeFunc(reshape);
	renderer->setIdleFunc(idle);
	renderer->setDisplayFunc(display);

	init();

	renderer->mainLoop(nCmdShow);

	kill();

	delete renderer;
	delete input;

	text = std::to_string(engine::Object::getMemoryState());
	//MessageBox(NULL, text.c_str(), "memState", MB_OK);
	engine::Object::saveMemoryInfo(L"memLost.txt");

	return 0;
}