#include "Demo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Logger::Instance().config(L"test_log.txt");

	Graphics::GraphicsRenderer::Instance().init(TEXT("Demo"), 800, 600, FALSE, hInstance, nCmdShow);

	std::shared_ptr<Demo> demo = std::shared_ptr<Demo>(new Demo);

	Graphics::GraphicsRenderer::Instance().mainLoop(demo);

	return 0;
}