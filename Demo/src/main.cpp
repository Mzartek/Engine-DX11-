#include "Demo.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Engine::Renderer *renderer = new Engine::Renderer(L"Demo DirectX", 800, 600, FALSE, hInstance, nCmdShow);
	Engine::Input *input = new Engine::Input(renderer->getWindow(), hInstance);

	Demo *game = new Demo(renderer, input);
	
	game->launch();
	
	delete game;

	delete input;
	delete renderer;

	if (Engine::Object::getMemoryState() != 0)
	{
		std::string text;
		text = std::to_string(Engine::Object::getMemoryState());
		MessageBoxA(NULL, text.c_str(), __FILE__, MB_OK);
	}

	return 0;
}