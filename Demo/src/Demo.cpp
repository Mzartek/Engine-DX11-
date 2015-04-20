#include "Demo.hpp"

Demo::Demo(Engine::Renderer *renderer, Engine::Input *input)
{
	this->renderer = renderer;
	this->input = input;

	skyboxProgram = new Engine::ShaderProgram(
		L"../shader/skybox/skyboxVertex.hlsl", 
		NULL, 
		NULL, 
		NULL, 
		L"../shader/skybox/skyboxPixel.hlsl");

	objectProgram = new Engine::ShaderProgram(
		L"../shader/object/objectVertex.hlsl", 
		NULL, 
		NULL, 
		L"../shader/object/objectGeom.hlsl", 
		L"../shader/object/objectPixel.hlsl");

	reflectObjectProgram = new Engine::ShaderProgram(
		L"../shader/reflectObject/reflectObjectVertex.hlsl", 
		NULL, 
		NULL, 
		L"../shader/reflectObject/reflectObjectGeom.hlsl", 
		L"../shader/reflectObject/reflectObjectPixel.hlsl");

	dirLightProgram = new Engine::ShaderProgram(
		L"../shader/dirLight/dirLightVertex.hlsl", 
		NULL, 
		NULL, 
		NULL, 
		L"../shader/dirLight/dirLightPixel.hlsl");

	spotLightProgram = new Engine::ShaderProgram(
		L"../shader/spotLight/spotLightVertex.hlsl", 
		NULL, 
		NULL, 
		NULL, 
		L"../shader/spotLight/spotLightPixel.hlsl");

	depthMapProgram = new Engine::ShaderProgram(
		L"../shader/depthMap/depthMapVertex.hlsl", 
		NULL, 
		NULL, 
		NULL, 
		L"../shader/depthMap/depthMapPixel.hlsl");

	backgroundProgram = new Engine::ShaderProgram(
		L"../shader/background/backgroundVertex.hlsl", 
		NULL, 
		NULL, 
		NULL, 
		L"../shader/background/backgroundPixel.hlsl");

	screenProgram = new Engine::ShaderProgram(
		L"../shader/screen/screenVertex.hlsl", 
		NULL, 
		NULL, 
		NULL, 
		L"../shader/screen/screenPixel.hlsl");

	// Buffers
	gBuffer = new Engine::GBuffer;
	dMaps = new Engine::DepthMap[3];

	cam = new Engine::FreeCam;
	skybox = new Engine::SkyBox(skyboxProgram);
	ground = new Ground(objectProgram, depthMapProgram);
	heli = new Engine::Model(reflectObjectProgram, depthMapProgram);
	sun = new Engine::DirLight(dirLightProgram);
	torch = new Engine::SpotLight(spotLightProgram);
	screen = new Engine::Screen(backgroundProgram, screenProgram);

	octreeSystem = new Engine::OctreeSystem(4, XMVectorSet(0, 0, 0, 0), 1000);

	// Buffers config
	gBuffer->config(renderer->getWidth(), renderer->getHeight());
	dMaps[0].config(2048, 2048);
	dMaps[1].config(2048, 2048);
	dMaps[2].config(2048, 2048);

	// Camera config
	cam->setCameraPosition(XMVectorSet(30, 5, 0, 1));

	// Skybox config
	skybox->load(
		"../resources/textures/skybox/rightred2.jpg", "../resources/textures/skybox/leftred2.jpg",
		"../resources/textures/skybox/topred2.jpg", "../resources/textures/skybox/botred2.jpg",
		"../resources/textures/skybox/frontred2.jpg", "../resources/textures/skybox/backred2.jpg");

	octreeSystem->addModel(ground->getModel(), 1000);

	heli->loadFromFile("../resources/models/heli/corps.mobj");
	heli->sortMesh();
	heli->setPosition(XMVectorSet(0.0f, 6.0f, 0.0f, 1.0f));
	heli->setScale(XMVectorSet(2.0f, 2.0f, 2.0f, 1.0f));
	heli->setCubeTexture(skybox->getTexture());

	octreeSystem->addModel(heli, 1000);

	// Light config
	sun->setColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	sun->setDirection(XMVectorSet(0.5f, -1.0f, 0.0f, 1.0f));

	torch->setColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	torch->setPosition(XMVectorSet(20.0f, 40.0f, 0.0f, 1.0f));
	torch->setDirection(XMVectorSet(-0.5f, -1.0f, 0.0f, 1.0f));
	torch->setSpotCutOff(XM_PI / 4);
	torch->setMaxDistance(100);
}

Demo::~Demo(void)
{
	delete octreeSystem;

	delete screen;
	delete torch;
	delete sun;
	delete heli;
	delete ground;
	delete skybox;
	delete cam;

	delete[] dMaps;
	delete gBuffer;

	delete screenProgram;
	delete backgroundProgram;
	delete depthMapProgram;
	delete spotLightProgram;
	delete dirLightProgram;
	delete reflectObjectProgram;
	delete objectProgram;
	delete skyboxProgram;
}

void Demo::display(FLOAT state)
{
	UNREFERENCED_PARAMETER(state);

	static std::set<Engine::Model *> object;

	// We retrieve object to display from the octree
	object.clear();
	octreeSystem->getModels(gBuffer, cam, &object);

	// Clear Buffers
	renderer->clear();
	gBuffer->clear();

	// Skybox
	skybox->display(gBuffer, cam);

	// Opaque Object
	for (std::set<Engine::Model *>::iterator it = object.begin(); it != object.end(); it++)
		(*it)->display(gBuffer, cam);

	// Lights
	dMaps[0].clear();
	dMaps[1].clear();
	dMaps[2].clear();
	heli->displayDepthMap(dMaps, sun);
	sun->display(gBuffer, dMaps, cam);

	dMaps[0].clear();
	heli->displayDepthMap(dMaps, torch);
	torch->display(gBuffer, dMaps, cam);

	screen->background(gBuffer);

	// Transparent Object
	for (std::set<Engine::Model *>::iterator it = object.begin(); it != object.end(); it++)
		(*it)->displayTransparent(gBuffer, cam);

	// Lights
	sun->display(gBuffer, cam);
	torch->display(gBuffer, cam);

	screen->background(gBuffer);

	screen->display(renderer, gBuffer, 1.0f, 1.0f, 1.0f, 1.0f);
}

void Demo::idle(long long time)
{
	UNREFERENCED_PARAMETER(time);

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
	torch->position(dMaps);
}

void Demo::reshape(UINT width, UINT height)
{
	cam->setPerspective(XM_PI / 2, width, height, 0.1f, 1000.0f);
}

void Demo::launch(void)
{
	renderer->mainLoop(this);
}