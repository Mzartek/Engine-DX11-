#include "config.hpp"

GameManager::GameManager(Engine::Renderer *r, Engine::Input *i)
{
	renderer = r;
	input = i;

	skyboxProgram = new Engine::ShaderProgram(TEXT("shader/skybox/skyboxVertex.hlsl"), NULL, NULL, NULL, TEXT("shader/skybox/skyboxPixel.hlsl"));
	objectProgram = new Engine::ShaderProgram(TEXT("shader/object/objectVertex.hlsl"), NULL, NULL, TEXT("shader/object/objectGeom.hlsl"), TEXT("shader/object/objectPixel.hlsl"));
	dirLightProgram = new Engine::ShaderProgram(TEXT("shader/dirLight/dirLightVertex.hlsl"), NULL, NULL, NULL, TEXT("shader/dirLight/dirLightPixel.hlsl"));
	spotLightProgram = new Engine::ShaderProgram(TEXT("shader/spotLight/spotLightVertex.hlsl"), NULL, NULL, NULL, TEXT("shader/spotLight/spotLightPixel.hlsl"));
	shadowMapProgram = new Engine::ShaderProgram(TEXT("shader/shadowMap/shadowMapVertex.hlsl"), NULL, NULL, NULL, TEXT("shader/shadowMap/shadowMapPixel.hlsl"));
	backgroundProgram = new Engine::ShaderProgram(TEXT("shader/background/backgroundVertex.hlsl"), NULL, NULL, NULL, TEXT("shader/background/backgroundPixel.hlsl"));
	screenProgram = new Engine::ShaderProgram(TEXT("shader/screen/screenVertex.hlsl"), NULL, NULL, NULL, TEXT("shader/screen/screenPixel.hlsl"));

	gBuffer = new Engine::GBuffer;
	cam = new Engine::FreeCam;
	skybox = new Engine::SkyBox(skyboxProgram);
	sol = new Engine::Model(objectProgram, shadowMapProgram);
	heli = new Engine::Model(objectProgram, shadowMapProgram);
	sun = new Engine::DirLight(dirLightProgram);
	torch = new Engine::SpotLight(spotLightProgram);
	screen = new Engine::Screen(backgroundProgram, screenProgram);

	// GBuffer config
	gBuffer->config(renderer->getWidth(), renderer->getHeight());

	// Camera config
	cam->setPositionCamera(XMFLOAT3(30, 10, 0));
	cam->setInitialAngle(-90, 0);

	// Skybox config
	skybox->load("resources/Skybox/rightred2.jpg", "resources/Skybox/leftred2.jpg",
		"resources/Skybox/topred2.jpg", "resources/Skybox/botred2.jpg",
		"resources/Skybox/frontred2.jpg", "resources/Skybox/backred2.jpg");
	skybox->rotate(180, 1, 0, 0);

	// Model config
	FLOAT vertexArray[] =
	{
		-500, 0, -500, 0, 0, 0, 1, 0, 1, 0, 0,
		-500, 0, 500, 0, 1, 0, 1, 0, 1, 0, 0,
		500, 0, 500, 1, 1, 0, 1, 0, 1, 0, 0,
		500, 0, -500, 1, 0, 0, 1, 0, 1, 0, 0
	};
	UINT index[] = { 2, 0, 1, 0, 2, 3 };
	XMFLOAT4 mat_ambient(0.5f, 0.5f, 0.5f, 1.0f);
	XMFLOAT4 mat_diffuse(0.9f, 0.9f, 0.9f, 1.0f);
	XMFLOAT4 mat_specular(1.0f, 1.0f, 1.0f, 1.0f);
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
	sun->setColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	sun->setDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	sun->setShadowMapping(TRUE);
	sun->configShadowMap(1024, 1024);

	torch->setColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	torch->setPosition(XMFLOAT3(20.0f, 40.0f, 0.0f));
	torch->setDirection(XMFLOAT3(-0.5f, -1.0f, 0.0f));
	torch->setSpotCutOff(45.0f);
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