#include "include/config.h"

// Globales variables
engine::Window*				window = NULL;
engine::Camera*				cam = NULL;
engine::Model*				sol = NULL;
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

	sol->display(window, cam);

	window->getSwapChain()->Present(0, 0);
}

void idle(void)
{
	cam->setPositionCamera(5.0f, 5.0f, -5.0f);
	cam->setPositionTarget(0.0f, 0.0f, 0.0f);

	//sol->matRotate(0.001f, FALSE, FALSE, TRUE);
}

void deleteClass()
{
	delete mainProgram;
	delete sol;
	delete cam;
	delete window;
}

void init(void)
{
	window = new engine::Window;
	cam = new engine::Camera;
	sol = new engine::Model;
	mainProgram = new engine::ShaderProgram;

	cam->setPerspective(90.0f, window->getWidth(), window->getHeight(), 0.01f, 1500.0f);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	init();

	if (FAILED(window->initWindow(hInstance, WndProc, "Demo Class", "Demo DirectX", 1024, 768)))
		return 1;
	if (FAILED(configShader()))
		return 1;
	if (FAILED(configSol()))
		return 1;

	window->setIdleFunc(idle);
	window->setDisplayFunc(display);
	window->mainLoop(nCmdShow);

	deleteClass();

	return 0;
}