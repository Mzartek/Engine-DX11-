#include <iostream>
#include "include/graphics.h"

// Globales variables
engine::Window*				window = NULL;
engine::Camera*				cam = NULL;
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

	window->clear();


	window->getSwapChain()->Present(0, 0);
}

void idle(void)
{
	static FLOAT i = 0;
	ConstantBuffer cb;
	DirectX::XMMATRIX modelMatrix;
	cam->setPositionCamera(5.0f, 5.0f, -5.0f);
	cam->setPositionTarget(0.0f, 0.0f, 0.0f);

	modelMatrix = DirectX::XMMatrixIdentity();

	cam->position();
	cb.MVP = *(cam->getMatrix()) * XMMatrixTranspose(modelMatrix);
	cb.MVP = cb.MVP * XMMatrixTranspose(DirectX::XMMatrixRotationZ(i));
	i += 0.001f;

}

void CleanupDevice()
{
	delete mainProgram;
	delete cam;
	delete window;
}

void init(void)
{
	window = new engine::Window;
	cam = new engine::Camera;
	mainProgram = new engine::ShaderProgram;

	cam->setPerspective(90, window->getWidth(), window->getHeight(), 0.01f, 1500.0f);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	init();

	if (FAILED(window->initWindow(hInstance, WndProc, L"Demo Class", L"Demo DirectX", 1024, 768)))
		return 1;
	if (FAILED(mainProgram->loadProgram(L"shader/mainVertex.hlsl", L"shader/mainPixel.hlsl", window->getD3DDevice())))
		return 1;
	if (FAILED(initShader()))
		return 1;
	if (FAILED(initBuffer()))
		return 1;
	if (FAILED(engine::loadTextureFromFile("ressources/test.png", &pTexture, &pSampler, window->getD3DDevice())))
		return 1;

	window->setIdleFunc(idle);
	window->setDisplayFunc(display);
	window->mainLoop(nCmdShow);

	CleanupDevice();

	return 0;
}