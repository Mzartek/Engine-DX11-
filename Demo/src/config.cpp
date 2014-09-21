#include "config.hpp"

void configShaders(void)
{
	objectProgram->loadProgram(L"shader/object/objectVertex.hlsl", NULL, NULL, L"shader/object/objectGeom.hlsl", L"shader/object/objectPixel.hlsl");
	dirLightProgram->loadProgram(L"shader/dirLight/dirLightVertex.hlsl", NULL, NULL, NULL, L"shader/dirLight/dirLightPixel.hlsl");
	spotLightProgram->loadProgram(L"shader/spotLight/spotLightVertex.hlsl", NULL, NULL, NULL, L"shader/spotLight/spotLightPixel.hlsl");
	shadowMapProgram->loadProgram(L"shader/shadowMap/shadowMapVertex.hlsl", NULL, NULL, NULL, L"shader/shadowMap/shadowMapPixel.hlsl");
	skyboxProgram->loadProgram(L"shader/skybox/skyboxVertex.hlsl", NULL, NULL, NULL, L"shader/skybox/skyboxPixel.hlsl");
	backgroundProgram->loadProgram(L"shader/background/backgroundVertex.hlsl", NULL, NULL, NULL, L"shader/background/backgroundPixel.hlsl");
	screenProgram->loadProgram(L"shader/screen/screenVertex.hlsl", NULL, NULL, NULL, L"shader/screen/screenPixel.hlsl");
}

void configBuffers(void)
{
	gBuffer->config(renderer->getWidth(), renderer->getHeight());
}

void configLights(void)
{
	sun->config(dirLightProgram);
	sun->setColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	sun->setDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	sun->setShadowMapping(TRUE);
	sun->configShadowMap(1024, 1024);

	torch->config(spotLightProgram);
	torch->setColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	torch->setPosition(XMFLOAT3(20.0f, 40.0f, 0.0f));
	torch->setDirection(XMFLOAT3(-0.5f, -1.0f, 0.0f));
	torch->setSpotCutOff(45.0f);
	torch->setShadowMapping(TRUE);
	torch->configShadowMap(1024, 1024);
}

void configScreen(void)
{
	screen->config(backgroundProgram, screenProgram);
}

void configModels(void)
{
	XMFLOAT4 mat_ambient(0.5f, 0.5f, 0.5f, 1.0f);
	XMFLOAT4 mat_diffuse(0.9f, 0.9f, 0.9f, 1.0f);
	XMFLOAT4 mat_specular(1.0f, 1.0f, 1.0f, 1.0f);
	FLOAT mat_shininess = 20.0f;

	FLOAT vertexArray[] = 
	{
			-500, 0, -500, 0, 0, 0, 1, 0, 1, 0, 0,
			-500, 0, 500, 0, 1, 0, 1, 0, 1, 0, 0,
			500, 0, 500, 1, 1, 0, 1, 0, 1, 0, 0,
			500, 0, -500, 1, 0, 0, 1, 0, 1, 0, 0
	};
	UINT index[] = { 2, 0, 1, 0, 2, 3 };

	sol->config(objectProgram, shadowMapProgram);
	sol->initMeshArray();
	sol->addMesh(sizeof vertexArray, vertexArray,
		sizeof index, index,
		"resources/ornaments.jpg", "resources/NM_none.png",
		mat_ambient, mat_diffuse, mat_specular, mat_shininess);

	// Heli
	heli->config(objectProgram, shadowMapProgram);
	heli->initMeshArray();
	heli->loadFromFile("resources/heli/corps.mobj");
	heli->sortMesh();
	heli->matTranslate(0.0f, 6.0f, 0.0f);
	heli->matScale(2, 2, 2);
}

void configSkybox(void)
{
	skybox->load("resources/Skybox/rightred2.jpg", "resources/Skybox/leftred2.jpg",
		"resources/Skybox/topred2.jpg", "resources/Skybox/botred2.jpg",
		"resources/Skybox/frontred2.jpg", "resources/Skybox/backred2.jpg",
		10, skyboxProgram);
	skybox->rotate(180, 1, 0, 0);
}