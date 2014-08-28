#include "include/config.hpp"

void configShaders(void)
{

	gObjectProgram->loadProgram(L"shader/object/gObjectVertex.hlsl", NULL, NULL, NULL, L"shader/object/gObjectPixel.hlsl", renderer->getD3DDevice());
	dirLightProgram->loadProgram(L"shader/dirLight/gDirLightVertex.hlsl", NULL, NULL, NULL, L"shader/dirLight/gDirLightPixel.hlsl", renderer->getD3DDevice());
	spotLightProgram->loadProgram(L"shader/spotLight/gSpotLightVertex.hlsl", NULL, NULL, NULL, L"shader/spotLight/gSpotLightPixel.hlsl", renderer->getD3DDevice());
	shadowProgram->loadProgram(L"shader/shadow/shadowVertex.hlsl", NULL, NULL, NULL, L"shader/shadow/shadowPixel.hlsl", renderer->getD3DDevice());
	gSkyboxProgram->loadProgram(L"shader/skybox/gSkyboxVertex.hlsl", NULL, NULL, NULL, L"shader/skybox/gSkyboxPixel.hlsl", renderer->getD3DDevice());
	screenProgram->loadProgram(L"shader/screen/screenVertex.hlsl", NULL, NULL, NULL, L"shader/screen/screenPixel.hlsl", renderer->getD3DDevice());
}

void configBuffers(void)
{
	gBuffer->config(renderer->getWidth(), renderer->getHeight(), renderer->getD3DDevice(), renderer->getContext());
}

void configLights(void)
{
	sun->config(dirLightProgram, renderer->getD3DDevice(), renderer->getContext());
	sun->setColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	sun->setDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	sun->activateShadowMapping(TRUE);
	sun->configShadowMap(1024, 1024, shadowProgram, renderer->getD3DDevice(), renderer->getContext());

	torch->config(spotLightProgram, renderer->getD3DDevice(), renderer->getContext());
	torch->setColor(XMFLOAT3(1.0f, 1.0f, 1.0f));
	torch->setPosition(XMFLOAT3(20.0f, 40.0f, 0.0f));
	torch->setDirection(XMFLOAT3(-0.5f, -1.0f, 0.0f));
	torch->setSpotCutOff(45.0f);
	torch->activateShadowMapping(TRUE);
	torch->configShadowMap(1024, 1024, shadowProgram, renderer->getD3DDevice(), renderer->getContext());
}

void configScreen(void)
{
	screen->config(screenProgram, renderer->getD3DDevice());
}

void configModels(void)
{
	XMFLOAT4 mat_ambient(0.5f, 0.5f, 0.5f, 1.0f);
	XMFLOAT4 mat_diffuse(0.9f, 0.9f, 0.9f, 1.0f);
	XMFLOAT4 mat_specular(1.0f, 1.0f, 1.0f, 1.0f);
	FLOAT mat_shininess = 20.0f;

	engine::Vertex vertexArray[] = {
			{ XMFLOAT3(-500, 0, -500), XMFLOAT2(0, 0), XMFLOAT3(0, 1, 0) },
			{ XMFLOAT3(-500, 0, 500), XMFLOAT2(0, 1), XMFLOAT3(0, 1, 0) },
			{ XMFLOAT3(500, 0, 500), XMFLOAT2(1, 1), XMFLOAT3(0, 1, 0) },
			{ XMFLOAT3(500, 0, -500), XMFLOAT2(1, 0), XMFLOAT3(0, 1, 0) }
	};
	UINT index[] = { 0, 2, 1, 0, 2, 3 };

	sol->config(gObjectProgram, renderer->getD3DDevice(), renderer->getContext());
	sol->initD3DObjectArray();
	sol->createObject(sizeof(vertexArray), (FLOAT *)vertexArray,
		sizeof index, index,
		"resources/ornaments.jpg",
		mat_ambient, mat_diffuse, mat_specular, mat_shininess);

	// Heli
	heli->config(gObjectProgram, renderer->getD3DDevice(), renderer->getContext());
	heli->initD3DObjectArray();
	heli->loadFromFile("resources/heli/corps.obj");
	heli->sortD3DObject();
	heli->matTranslate(0.0f, 6.0f, 0.0f);
	heli->matScale(2, 2, 2);
}

void configSkybox(void)
{
	skybox->load("resources/Skybox/rightred2.jpg", "resources/Skybox/leftred2.jpg",
		"resources/Skybox/topred2.jpg", "resources/Skybox/botred2.jpg",
		"resources/Skybox/frontred2.jpg", "resources/Skybox/backred2.jpg",
		10, gSkyboxProgram, renderer->getD3DDevice());
	skybox->rotate(180, 1, 0, 0);
}