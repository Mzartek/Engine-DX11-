#include "config.hpp"

void GameManager::display(FLOAT state)
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

void GameManager::idle(void)
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

void GameManager::reshape(UINT width, UINT height)
{
	cam->setPerspective(90.0f, width, height, 0.1f, 1000.0f);
}

void GameManager::launch(void)
{
	renderer->mainLoop(this);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Engine::Renderer *renderer = new Engine::Renderer(L"Demo DirectX", 800, 600, FALSE, hInstance, nCmdShow);
	Engine::Input *input = new Engine::Input(renderer->getWindow(), hInstance);
	GameManager *game = new GameManager(renderer, input);
	
	game->launch();
	
	delete game;
	delete renderer;
	delete input;

	if (Engine::Object::getMemoryState() != 0)
	{
		std::string text;
		text = std::to_string(Engine::Object::getMemoryState());
		MessageBoxA(NULL, text.c_str(), __FILE__, MB_OK);
	}
	Engine::Object::saveMemoryInfo("memLost.txt");

	return 0;
}