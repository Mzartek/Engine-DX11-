#include "include/config.hpp"

void configShader(void)
{

	gObjectProgram->loadProgram(L"shader/object/gObjectVertex.hlsl", NULL, NULL, NULL, L"shader/object/gObjectPixel.hlsl", renderer->getD3DDevice());
	gSkyboxProgram->loadProgram(L"shader/skybox/gSkyboxVertex.hlsl", NULL, NULL, NULL, L"shader/skybox/gSkyboxPixel.hlsl", renderer->getD3DDevice());
	screenProgram->loadProgram(L"shader/screen/screenVertex.hlsl", NULL, NULL, NULL, L"shader/screen/screenPixel.hlsl", renderer->getD3DDevice());
}

void configBuffer(void)
{
	gBuffer->config(renderer->getWidth(), renderer->getHeight(), renderer->getD3DDevice(), renderer->getContext());
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

void configScreen(void)
{
	screen->config(screenProgram, renderer->getD3DDevice());
}

void configSkyBox(void)
{
	skybox->load("resources/Skybox/rightred2.jpg", "resources/Skybox/leftred2.jpg",
		"resources/Skybox/topred2.jpg", "resources/Skybox/botred2.jpg",
		"resources/Skybox/frontred2.jpg", "resources/Skybox/backred2.jpg",
		10, gSkyboxProgram, renderer->getD3DDevice());
	skybox->rotate(180, 1, 0, 0);
}