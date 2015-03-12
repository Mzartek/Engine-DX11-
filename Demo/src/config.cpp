#include "config.hpp"

GameManager::GameManager(Engine::Renderer *renderer, Engine::Input *input)
{
	this->renderer = renderer;
	this->input = input;

	skyboxProgram = new Engine::ShaderProgram(L"shader/skybox/skyboxVertex.hlsl", NULL, NULL, NULL, L"shader/skybox/skyboxPixel.hlsl");
	objectProgram = new Engine::ShaderProgram(L"shader/object/objectVertex.hlsl", NULL, NULL, L"shader/object/objectGeom.hlsl", L"shader/object/objectPixel.hlsl");
	reflectObjectProgram = new Engine::ShaderProgram(L"shader/reflectObject/reflectObjectVertex.hlsl", NULL, NULL, L"shader/reflectObject/reflectObjectGeom.hlsl", L"shader/reflectObject/reflectObjectPixel.hlsl");
	dirLightProgram = new Engine::ShaderProgram(L"shader/dirLight/dirLightVertex.hlsl", NULL, NULL, NULL, L"shader/dirLight/dirLightPixel.hlsl");
	spotLightProgram = new Engine::ShaderProgram(L"shader/spotLight/spotLightVertex.hlsl", NULL, NULL, NULL, L"shader/spotLight/spotLightPixel.hlsl");
	shadowMapProgram = new Engine::ShaderProgram(L"shader/shadowMap/shadowMapVertex.hlsl", NULL, NULL, NULL, L"shader/shadowMap/shadowMapPixel.hlsl");
	backgroundProgram = new Engine::ShaderProgram(L"shader/background/backgroundVertex.hlsl", NULL, NULL, NULL, L"shader/background/backgroundPixel.hlsl");
	screenProgram = new Engine::ShaderProgram(L"shader/screen/screenVertex.hlsl", NULL, NULL, NULL, L"shader/screen/screenPixel.hlsl");

	gBuffer = new Engine::GBuffer();
	cam = new Engine::FreeCam;
	skybox = new Engine::SkyBox(skyboxProgram);
	sol = new Engine::Model(objectProgram, shadowMapProgram);
	heli = new Engine::Model(reflectObjectProgram, shadowMapProgram);
	sun = new Engine::DirLight(dirLightProgram);
	torch = new Engine::SpotLight(spotLightProgram);
	screen = new Engine::Screen(backgroundProgram, screenProgram);

	octreeSystem = new Engine::OctreeSystem(4, XMVectorSet(0, 0, 0, 0), 1000);

	// GBuffer config
	gBuffer->config(renderer->getWidth(), renderer->getHeight());

	// Camera config
	cam->setCameraPosition(XMVectorSet(30, 5, 0, 1));
	cam->setInitialAngle(-XM_PI / 2, 0);

	// Skybox config
	skybox->load("resources/Skybox/rightred2.jpg", "resources/Skybox/leftred2.jpg",
		"resources/Skybox/topred2.jpg", "resources/Skybox/botred2.jpg",
		"resources/Skybox/frontred2.jpg", "resources/Skybox/backred2.jpg");

	// Model config
	Engine::Vertex vertexArray[] =
	{
		XMFLOAT3( -500, 0, -500 ), XMFLOAT2( 0, 0 ), XMFLOAT3( 0, 1, 0 ), XMFLOAT3( 1, 0, 0 ),
		XMFLOAT3( -500, 0, 500 ), XMFLOAT2( 0, 1 ), XMFLOAT3( 0, 1, 0 ), XMFLOAT3( 1, 0, 0 ),
		XMFLOAT3( 500, 0, 500 ), XMFLOAT2( 1, 1 ), XMFLOAT3( 0, 1, 0 ), XMFLOAT3( 1, 0, 0 ),
		XMFLOAT3( 500, 0, -500 ), XMFLOAT2( 1, 0 ), XMFLOAT3( 0, 1, 0 ), XMFLOAT3( 1, 0, 0 ),
	};
	UINT index[] = { 2, 0, 1, 0, 2, 3 };
	XMVECTOR mat_ambient = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
	XMVECTOR mat_diffuse = XMVectorSet(0.9f, 0.9f, 0.9f, 1.0f);
	XMVECTOR mat_specular = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	FLOAT mat_shininess = 20.0f;

	sol->addMesh(sizeof vertexArray / sizeof(Engine::Vertex), vertexArray,
		sizeof index / sizeof(UINT), index,
		"resources/ornaments.jpg", "resources/NM_none.png",
		mat_ambient, mat_diffuse, mat_specular, mat_shininess);

	octreeSystem->addModel(sol, 1000);

	heli->loadFromFile("resources/heli/corps.mobj");
	heli->sortMesh();
	heli->setPosition(XMVectorSet(0.0f, 6.0f, 0.0f, 1.0f));
	heli->setScale(XMVectorSet(2.0f, 2.0f, 2.0f, 1.0f));
	heli->setCubeTexture(skybox->getTexture());

	octreeSystem->addModel(heli, 1000);

	// Light config
	sun->setColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	sun->setDirection(XMVectorSet(0.5f, -1.0f, 0.0f, 1.0f));
	sun->setShadowMapping(TRUE);
	sun->configShadowMap(2048, 2048);

	torch->setColor(XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	torch->setPosition(XMVectorSet(20.0f, 40.0f, 0.0f, 1.0f));
	torch->setDirection(XMVectorSet(-0.5f, -1.0f, 0.0f, 1.0f));
	torch->setSpotCutOff(XM_PI / 4);
	torch->setMaxDistance(100);
	torch->setShadowMapping(TRUE);
	torch->configShadowMap(2048, 2048);
}

GameManager::~GameManager(void)
{
	delete octreeSystem;

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
	delete reflectObjectProgram;
	delete objectProgram;
	delete skyboxProgram;
}