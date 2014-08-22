#include "include/config.hpp"

HRESULT configShader(void)
{
	HRESULT hr;

	hr = gObjectProgram->loadProgram(L"shader/object/gObjectVertex.hlsl", NULL, L"shader/object/gObjectPixel.hlsl", renderer->getD3DDevice());
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed gBufferProgram", "ShaderProgram", MB_OK);
		return hr;
	}

	hr = gSkyboxProgram->loadProgram(L"shader/skybox/gSkyboxVertex.hlsl", NULL, L"shader/skybox/gSkyboxPixel.hlsl", renderer->getD3DDevice());
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed gSkyBoxProgram", "ShaderProgram", MB_OK);
		return hr;
	}

	hr = screenProgram->loadProgram(L"shader/screen/screenVertex.hlsl", NULL, L"shader/screen/screenPixel.hlsl", renderer->getD3DDevice());
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed screenProgram.", "ShaderProgram", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT configBuffer(void)
{
	return gBuffer->config(renderer->getWidth(), renderer->getHeight(), renderer->getD3DDevice());
}

HRESULT configModels(void)
{
	HRESULT hr;

	DirectX::XMFLOAT4 mat_ambient(0.2f, 0.2f, 0.2f, 1.0f);
	DirectX::XMFLOAT4 mat_diffuse(0.8f, 0.8f, 0.8f, 1.0f);
	DirectX::XMFLOAT4 mat_specular(1.0f, 1.0f, 1.0f, 1.0f);
	FLOAT mat_shininess = 20.0f;

	engine::Vertex vertexArray[] = {
			{ DirectX::XMFLOAT3(-500, 0, -500), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(-500, 0, 500), DirectX::XMFLOAT2(0, 1), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(500, 0, 500), DirectX::XMFLOAT2(1, 1), DirectX::XMFLOAT3(0, 1, 0) },
			{ DirectX::XMFLOAT3(500, 0, -500), DirectX::XMFLOAT2(1, 0), DirectX::XMFLOAT3(0, 1, 0) }
	};
	UINT index[] = { 0, 2, 1, 0, 2, 3 };

	hr = sol->config(gObjectProgram, renderer->getD3DDevice(), renderer->getImmediateContext());
	if (FAILED(hr))
		return hr;
	sol->initD3DObjectArray();
	hr = sol->createObject(sizeof(vertexArray), (FLOAT *)vertexArray,
		sizeof index, index,
		"resources/test.png",
		mat_ambient, mat_diffuse, mat_specular, mat_shininess);
	if (FAILED(hr))
		return hr;

	// Heli
	hr = heli->config(gObjectProgram, renderer->getD3DDevice(), renderer->getImmediateContext());
	if (FAILED(hr))
		return hr;
	heli->initD3DObjectArray();
	hr = heli->loadFromFile("resources/heli/corps.obj");
	if (FAILED(hr))
		return hr;
	heli->sortD3DObject();
	heli->matTranslate(0.0f, 6.0f, 0.0f);
	heli->matScale(2, 2, 2);

	return S_OK;
}

HRESULT configScreen(void)
{
	return screen->config(screenProgram, renderer->getD3DDevice(), renderer->getImmediateContext());
}

HRESULT configSkyBox(void)
{
	HRESULT hr;

	hr = skybox->load("resources/Skybox/rightred2.jpg", "resources/Skybox/leftred2.jpg",
		"resources/Skybox/topred2.jpg", "resources/Skybox/botred2.jpg",
		"resources/Skybox/frontred2.jpg", "resources/Skybox/backred2.jpg",
		10, 
		gSkyboxProgram, renderer->getD3DDevice(), renderer->getImmediateContext());
	if (FAILED(hr))
		return hr;

	skybox->rotate(180, 1, 0, 0);

	return S_OK;
}