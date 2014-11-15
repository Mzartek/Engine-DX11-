#include "config.hpp"

GameManager::GameManager(Engine::Renderer *renderer, Engine::Input *input)
{
	this->renderer = renderer;
	this->input = input;

	skyboxProgram = new Engine::ShaderProgram(this->renderer->getEngineDevice(), L"shader/skybox/skyboxVertex.hlsl", NULL, NULL, NULL, L"shader/skybox/skyboxPixel.hlsl");
	objectProgram = new Engine::ShaderProgram(this->renderer->getEngineDevice(), L"shader/object/objectVertex.hlsl", NULL, NULL, L"shader/object/objectGeom.hlsl", L"shader/object/objectPixel.hlsl");
	dirLightProgram = new Engine::ShaderProgram(this->renderer->getEngineDevice(), L"shader/dirLight/dirLightVertex.hlsl", NULL, NULL, NULL, L"shader/dirLight/dirLightPixel.hlsl");
	spotLightProgram = new Engine::ShaderProgram(this->renderer->getEngineDevice(), L"shader/spotLight/spotLightVertex.hlsl", NULL, NULL, NULL, L"shader/spotLight/spotLightPixel.hlsl");
	shadowMapProgram = new Engine::ShaderProgram(this->renderer->getEngineDevice(), L"shader/shadowMap/shadowMapVertex.hlsl", NULL, NULL, NULL, L"shader/shadowMap/shadowMapPixel.hlsl");
	backgroundProgram = new Engine::ShaderProgram(this->renderer->getEngineDevice(), L"shader/background/backgroundVertex.hlsl", NULL, NULL, NULL, L"shader/background/backgroundPixel.hlsl");
	screenProgram = new Engine::ShaderProgram(this->renderer->getEngineDevice(), L"shader/screen/screenVertex.hlsl", NULL, NULL, NULL, L"shader/screen/screenPixel.hlsl");

	gBuffer = new Engine::GBuffer(this->renderer->getEngineDevice());
	cam = new Engine::FreeCam;
	skybox = new Engine::SkyBox(this->renderer->getEngineDevice(), skyboxProgram);
	sol = new Engine::Model(this->renderer->getEngineDevice(), objectProgram, shadowMapProgram);
	heli = new Engine::Model(this->renderer->getEngineDevice(), objectProgram, shadowMapProgram);
	sun = new Engine::DirLight(this->renderer->getEngineDevice(), dirLightProgram);
	torch = new Engine::SpotLight(this->renderer->getEngineDevice(), spotLightProgram);
	screen = new Engine::Screen(this->renderer->getEngineDevice(), backgroundProgram, screenProgram);

	// GBuffer config
	gBuffer->config(this->renderer->getWidth(), this->renderer->getHeight());

	// Camera config
	cam->setPositionCamera(XMVectorSet(30, 10, 0, 1));
	cam->setInitialAngle(-XM_PI / 2, 0);

	// Skybox config
	skybox->load("resources/Skybox/rightred2.jpg", "resources/Skybox/leftred2.jpg",
		"resources/Skybox/topred2.jpg", "resources/Skybox/botred2.jpg",
		"resources/Skybox/frontred2.jpg", "resources/Skybox/backred2.jpg");
	skybox->rotate(XM_PI, 1, 0, 0);

	// Model config
	FLOAT vertexArray[] =
	{
		-500, 0, -500, 0, 0, 0, 1, 0, 1, 0, 0,
		-500, 0, 500, 0, 1, 0, 1, 0, 1, 0, 0,
		500, 0, 500, 1, 1, 0, 1, 0, 1, 0, 0,
		500, 0, -500, 1, 0, 0, 1, 0, 1, 0, 0
	};
	UINT index[] = { 2, 0, 1, 0, 2, 3 };
	XMVECTOR mat_ambient = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
	XMVECTOR mat_diffuse = XMVectorSet(0.9f, 0.9f, 0.9f, 1.0f);
	XMVECTOR mat_specular = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	FLOAT mat_shininess = 20.0f;

	sol->initMeshArray();
	sol->addMesh(sizeof vertexArray, vertexArray,
		sizeof index, index,
		"resources/ornaments.jpg", "resources/NM_none.png",
		mat_ambient, mat_diffuse, mat_specular, mat_shininess);

	heli->initMeshArray();
	heli->loadFromFile("resources/heli/corps.mobj");
	heli->sortMesh();
	heli->matTranslate(0.0f, 6.0f, 0.0f);
	heli->matScale(2, 2, 2);

	// Light config
	sun->setColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	sun->setDirection(XMVectorSet(1.0f, -1.0f, 0.0f, 1.0f));
	sun->setShadowMapping(TRUE);
	sun->configShadowMap(1024, 1024);

	torch->setColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	torch->setPosition(XMVectorSet(20.0f, 40.0f, 0.0f, 1.0f));
	torch->setDirection(XMVectorSet(-0.5f, -1.0f, 0.0f, 1.0f));
	torch->setSpotCutOff(XM_PI / 4);
	torch->setShadowMapping(TRUE);
	torch->configShadowMap(1024, 1024);
}

GameManager::~GameManager(void)
{
	delete screen;
	delete torch;
	delete sun;
	delete heli;
	delete sol;
	delete skybox;
	delete cam;
	delete gBuffer;

	delete screenProgram;
	delete backgroundProgram;
	delete shadowMapProgram;
	delete spotLightProgram;
	delete dirLightProgram;
	delete objectProgram;
	delete skyboxProgram;
}