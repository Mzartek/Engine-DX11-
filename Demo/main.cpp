#include "include/config.hpp"

// Globales variables
engine::Window*				window = NULL;
engine::Input*				input = NULL;
engine::GBuffer*			gBuffer = NULL;
engine::FreeCam*			cam = NULL;
engine::Model*				sol = NULL;
engine::Model*				heli = NULL;
engine::ShaderProgram*		mainProgram = NULL;

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

	window->clear();

	heli->display(window, cam);
	sol->display(window, cam);
}

void idle(void)
{
	input->refresh();
	if (input->getKeyBoardState(DIK_ESCAPE))
		window->stopLoop();
	cam->keyboardMove(input->getKeyBoardState(DIK_W), input->getKeyBoardState(DIK_S), input->getKeyBoardState(DIK_A), input->getKeyBoardState(DIK_D));
	cam->mouseMove(input->getMouseRelX(), input->getMouseRelY());
	if (input->getKeyBoardState(DIK_LSHIFT))
		cam->setSpeed(0.05f);
	else
		cam->setSpeed(0.25f);

	heli->matRotate(0.1f, 0, 1, 0);
}

void init(void)
{
	gBuffer = new engine::GBuffer;
	cam = new engine::FreeCam;
	sol = new engine::Model;
	heli = new engine::Model;
	mainProgram = new engine::ShaderProgram;

	if (FAILED(configShader()))
	{
		MessageBox(NULL, "Error Config Shader", "Error", MB_OK);
		exit(1);
	}
	if (FAILED(configBuffer()))
	{
		MessageBox(NULL, "Error Config Buffer", "Error", MB_OK);
		exit(1);
	}
	if (FAILED(configModels()))
	{
		MessageBox(NULL, "Error Config Models", "Error", MB_OK);
		exit(1);
	}

	cam->setPerspective(90.0f, window->getWidth(), window->getHeight(), 0.1f, 1500.0f);
	cam->setPositionCamera(5.0f, 5.0f, -5.0f);
	cam->setPositionTarget(0.0f, 2.5f, 0.0f);
}

void kill()
{
	delete mainProgram;
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
	window = new engine::Window;
	input = new engine::Input;

	if (FAILED(window->initWindow(hInstance, WndProc, "Demo DirectX", 800, 600, FALSE)))
	{
		MessageBox(NULL, "Error while init window", "Error", MB_OK);
		return 1;
	}
	if (FAILED(input->initInput(hInstance, window->getHWND())))
	{
		MessageBox(NULL, "Error while init input", "Error", MB_OK);
		return 1;
	}
	window->setIdleFunc(idle);
	window->setDisplayFunc(display);

	init();

	window->mainLoop(nCmdShow);

	kill();

	delete window;
	delete input;

	text = std::to_string(engine::Object::getMemoryState());
	//MessageBox(NULL, text.c_str(), "memState", MB_OK);
	engine::Object::saveMemoryInfo(L"memLost.txt");

	return 0;
}