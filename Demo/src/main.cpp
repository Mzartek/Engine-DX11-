#include "config.hpp"

void GameManager::display(FLOAT state)
{
	static UINT i;
	static std::vector<Engine::Model *> object;

	// We retrieve object to display from the octree
	object.clear();
	octreeSystem->getModel(gBuffer, cam, &object);

	// Clear Buffers
	renderer->clear();
	gBuffer->clear();
	sun->clear();
	torch->clear();

	// Skybox
	skybox->display(gBuffer, cam);

	// Shadow Map
	heli->displayShadowMap(sun);
	heli->displayShadowMap(torch);

	// Opaque Object
	for (i = 0; i < object.size(); i++)
		object[i]->display(gBuffer, cam);

	sun->display(gBuffer, cam);
	torch->display(gBuffer, cam);
	screen->background(gBuffer);

	// Transparent Object
	for (i = 0; i < object.size(); i++)
		object[i]->displayTransparent(gBuffer, cam);

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

	heli->setRotation(heli->getRotation() + XMVectorSet(0.0f, 0.0025f, 0.0f, 1.0f));

	cam->position();
	sun->position(cam->getCameraPosition(), 100, 250, 500);
	torch->position();
}

void GameManager::reshape(UINT width, UINT height)
{
	cam->setPerspective(XM_PI / 2, width, height, 0.1f, 1000.0f);
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
	delete input;
	delete renderer;

	if (Engine::Object::getMemoryState() != 0)
	{
		std::string text;
		text = std::to_string(Engine::Object::getMemoryState());
		MessageBoxA(NULL, text.c_str(), __FILE__, MB_OK);
	}
	Engine::Object::saveMemoryInfo("memLost.txt");

	return 0;
}